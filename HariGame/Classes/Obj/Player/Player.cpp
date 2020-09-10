#include <math.h>
#include "Split/Split.h"
#include "Player.h"
#include "Scene/GameScene.h"
#include "HP/HPMng.h"
#include "Action/ActionConvey.h"

USING_NS_CC;

Obj* Player::createPlayer(OBJ_COLOR color)
{
	Obj* pRet = new(std::nothrow) Player();
	if (pRet && pRet->init())
	{
		SpriteFrameCache::getInstance()->addSpriteFrame(Sprite::create("player/player_r_rotate.png")->getSpriteFrame(), "rotate_r");
		SpriteFrameCache::getInstance()->addSpriteFrame(Sprite::create("player/player_g_rotate.png")->getSpriteFrame(), "rotate_g");

		SpriteFrameCache::getInstance()->addSpriteFrame(Sprite::create("player/player_r_jump.png")->getSpriteFrame(), "jump_r");
		SpriteFrameCache::getInstance()->addSpriteFrame(Sprite::create("player/player_g_jump.png")->getSpriteFrame(), "jump_g");

		SpriteFrameCache::getInstance()->addSpriteFrame(Sprite::create("player/player_r_damage.png")->getSpriteFrame(), "damage_r");
		SpriteFrameCache::getInstance()->addSpriteFrame(Sprite::create("player/player_g_damage.png")->getSpriteFrame(), "damage_g");

		if (color == OBJ_COLOR::OBJ_RED)
		{
			pRet->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("rotate_r"));
		}
		else if (color == OBJ_COLOR::OBJ_GREEN)
		{
			pRet->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("rotate_g"));
		}

		pRet->setTag(static_cast<int>(color));
		pRet->setAnchorPoint(cocos2d::Vec2(0.5f, 0.5f));
		pRet->setVisible(true);
		((Player*)pRet)->_color = color;

		pRet->autorelease();

		return pRet;
	}
	else
	{
		delete pRet;
		pRet = nullptr;
		return nullptr;
	}
}

Player::Player()
{
	_damageFlag = false;
	_gameOverFlag = false;
	_jumpFlag = false;
	_flowerFlag = false;

	_moveVec = 192.0f;
	_vector = 0.0f;
	_time = 0.0f;
	_maxVec = 24.0f;

	_action = ACTION::NON;

	_blackList[ACTION::JUMP].push_back(ACTION::JUMP);
	_blackList[ACTION::JUMP].push_back(ACTION::JUMPING);
	_blackList[ACTION::JUMP].push_back(ACTION::FALL);

	_blackList[ACTION::FALL].push_back(ACTION::JUMP);
	_blackList[ACTION::FALL].push_back(ACTION::JUMPING);

	_image[ACTION::ROTATE] = { "rotate_r","rotate_g" };
	_image[ACTION::DAMAGE] = { "damage_r","damage_g" };
	_image[ACTION::JUMP] = { "jump_r","jump_g" };

	_rollingAction = nullptr;
	_damageAction = nullptr;
	_gemeOverAction = nullptr;

	//@cricket
	_actionBank = nullptr;
	_jumpSE = nullptr;
	_damageSE = nullptr;
	_changeSE = nullptr;
}

Player::~Player()
{
	//@cricket
	if (_actionBank)
	{
		_actionBank->destroy();
		_actionBank = nullptr;
	}
	if (_jumpSE)
	{
		_jumpSE->destroy();
		_jumpSE = nullptr;
	}
	if (_damageSE)
	{
		_damageSE->destroy();
		_damageSE = nullptr;
	}
	if (_changeSE)
	{
		_changeSE->destroy();
		_changeSE = nullptr;
	}
}

bool Player::init()
{
	if (!Sprite::init())
	{
		return false;
	}

	//@cricket
#ifdef CK_PLATFORM_WIN
	_actionBank = CkBank::newBank("Resources/se/action/action.ckb");
#else
	_actionBank = CkBank::newBank("se/action/action.ckb");
#endif
	_jumpSE = CkSound::newBankSound(_actionBank, "jump");
	_damageSE = CkSound::newBankSound(_actionBank, "damage");
	_changeSE = CkSound::newBankSound(_actionBank, "change");
	_changeSE->setVolume(0.4f);
	return true;
}

void Player::update(float delta)
{
	if (Director::getInstance()->getRunningScene()->getName() != "GameScene")
	{
		return;
	}

	if (_gameOverFlag)
	{
		if (_gemeOverAction->isDone())
		{
			CC_SAFE_RELEASE_NULL(_gemeOverAction);
			unscheduleUpdate();
		}
		return;
	}

	if (!_gameOverFlag)
	{
		if (_action == ACTION::ROTATE)
		{
			Rotate();
		}

		if (_rollingAction != nullptr)
		{
			Rolling(delta);
		}

		if (_action == ACTION::JUMP)
		{
			Jump();
		}
		if (_action == ACTION::JUMPING)
		{
			Jumping();
		}

		// —Ž‰º
		Falling();
		// ÀÞÒ°¼Þ
		Damage();
	}
}

void Player::Rotate()
{
	if (_rollingAction == nullptr)
	{
		_rollingAction = runAction(
			RepeatForever::create(
				Spawn::create(
					RotateBy::create(1.0f, 360.0f),
					MoveBy::create(1.0f, Vec2(_moveVec, 0.0f)),
					nullptr)
		));
		CC_SAFE_RETAIN(_rollingAction);
		_action = ACTION::ROLLING;
	}
}

void Player::Jump()
{
	if (_gameOverFlag)
	{
		return;
	}

	if ((!_jumpFlag) && (_vector == 0.0f))
	{
		_jumpFlag = true;
		_flowerFlag = false;
		_airTime = 0.8f;
		_action = ACTION::JUMPING;
		SetImage(ACTION::JUMP);

		//@cricket
		if (getName() == "player_behind")
		{
			_jumpSE->setVolume(0.8f);
		}
		_jumpSE->play();
	}
	else
	{
		_action = ACTION::ROLLING;
	}
}

void Player::Falling()
{
	for (auto bl : _blackList[ACTION::FALL])
	{
		if (_action == bl)
		{
			return;
		}
	}

	if (_flowerFlag)
	{
		_jumpFlag = false;
		_time = 0.0f;
		_vector = 0.0f;
		if (!_damageFlag)
		{
			SetImage(ACTION::ROTATE);
		}
		return;
	}

	_vector = (9.8f * _time) / 2.0f;
	if (CollsionCheck(Vec2(0.0f, -_vector - _point.y)))
	{
		setPosition(Vec2(getPosition().x, getPosition().y - _vector));
		_time += 0.1f;
		_jumpFlag = true;
	}
	else
	{
		_jumpFlag = false;
		_time = 0.0f;
		_vector = 0.0f;
		if (!_damageFlag)
		{
			SetImage(ACTION::ROTATE);
		}
	}
}

void Player::Change(int color)
{
	if (_gameOverFlag)
	{
		return;
	}

	//@_cricket
	_changeSE->play();
	if (color == static_cast<int>(OBJ_COLOR::OBJ_RED))
	{
		setSpriteFrame(Sprite::create("player/player_r_rotate.png")->getSpriteFrame());
		setTag(static_cast<int>(OBJ_COLOR::OBJ_RED));
		_color = OBJ_COLOR::OBJ_RED;
	}
	else if (color == static_cast<int>(OBJ_COLOR::OBJ_GREEN))
	{
		setSpriteFrame(Sprite::create("player/player_g_rotate.png")->getSpriteFrame());
		setTag(static_cast<int>(OBJ_COLOR::OBJ_GREEN));
		_color = OBJ_COLOR::OBJ_GREEN;
	}
}

bool Player::CollsionCheck(cocos2d::Vec2 vec)
{
	cocos2d::TMXTiledMap* mapData = (cocos2d::TMXTiledMap*)Director::getInstance()->getRunningScene()->getChildByName("BG_LAYER")->getChildByName("MapData");
	cocos2d::TMXLayer* scaffoldLayer = mapData->getLayer("scaffold");

	auto mapSize = scaffoldLayer->getLayerSize();
	auto TileSize = mapData->getTileSize();

	auto checkPoint = Vec2{ floorf((this->getPosition().x + vec.x)/ TileSize.width),floorf((this->getPosition().y + vec.y) / TileSize.height) };
	checkPoint.y = mapSize.height - checkPoint.y;	// ã‰º”½“]

	if (checkPoint.x >= mapSize.width)
	{
		unscheduleUpdate();
		stopAllActions();
		return false;
	}
	// —Ž‚¿‚½ê‡
	if (checkPoint.y >= mapSize.height)
	{
		auto _hPMng = ((HPMng*)Director::getInstance()->getRunningScene()->getChildByName("HP"));
		_hPMng->DamageHP(_hPMng->GetHP());
		return false;
	}

	auto gid = scaffoldLayer->getTileGIDAt(checkPoint);
	if ((gid != 0))
	{
		if (gid == 213)
		{
			stopAllActions();
			CC_SAFE_RELEASE_NULL(_rollingAction);
			setPosition((checkPoint.x - 1) * TileSize.width + _point.x, this->getPosition().y);
			return false;
		}
		
		if (vec.y <= 0.0f)
		{
			while (gid != 0)
			{
				checkPoint.y--;
				gid = scaffoldLayer->getTileGIDAt(checkPoint);
			}

			checkPoint.y = mapSize.height - checkPoint.y;	// ã‰º”½“]
			setPosition(Vec2(getPosition().x, (checkPoint.y) * TileSize.height - _point.y + (TileSize.height / 2.0f)));
		}
		return false;
	}

	return true;
}

void Player::DamageAction(cocos2d::Sprite* spite)
{
	if ((!_damageFlag)||(spite == this))
	{
		//@cricket
		if (spite->getName() != "bomb")
		{
			_damageSE->play();
		}

		if ((spite->getName() == "blackLadydug")|| (spite->getName() == "bomb"))
		{
			_damageAction = runAction(Sequence::create(
				Blink::create(1.5f, 10),
				nullptr
				));
		}
		else
		{
			stopAllActions();
			CC_SAFE_RELEASE_NULL(_rollingAction);
			float time = 0.3f;
			if (getName() == "player_front")
			{
				auto behindPosX = Director::getInstance()->getRunningScene()->getChildByName("PLAYER_LAYER")->getChildByName("player_behind")->getPosition().x;
				_damageAction = runAction(Sequence::create(
					MoveTo::create(time - 0.1f, Vec2(behindPosX, 0.0f)),
					DelayTime::create(time - 0.2f),
					nullptr
				));
			}
			else if (getName() == "player_behind")
			{
				_damageAction = runAction(DelayTime::create(time));
			}
			// ±¸¼®Ý“`’B‚ÌØ¾¯Ä
			((ActionConvey*)Director::getInstance()->getRunningScene()->getChildByName("actionConvey"))->ConveyClear();
		}

		CC_SAFE_RETAIN(_damageAction);
		//_action = ACTION::DAMAGE;
		_damageFlag = true;
		SetImage(ACTION::DAMAGE);
		return;
	}	
}

void Player::GameOverAction()
{
	if (!_gameOverFlag)
	{
		// ¡‚Ì±¸¼®Ý‚ðŽ~‚ß‚é
		stopAllActions();
		CC_SAFE_RELEASE_NULL(_damageAction);
		CC_SAFE_RELEASE_NULL(_rollingAction);
		setVisible(true);

		// ¹Þ°Ñµ°ÊÞ°Žž‚Ì±¸¼®Ý‚ðs‚¤
		_gemeOverAction = runAction(FadeOut::create(1.0f));
		CC_SAFE_RETAIN(_gemeOverAction);
		_gameOverFlag = true;
	}
}

void Player::GameClearAction()
{
	// ¡‚Ì±¸¼®Ý‚ðŽ~‚ß‚é
	stopAllActions();
	CC_SAFE_RELEASE_NULL(_damageAction);
	CC_SAFE_RELEASE_NULL(_rollingAction);

	unscheduleUpdate();

	setVisible(false);
}

void Player::Resume()
{
	if (_damageAction)
	{
		runAction(_damageAction);
	}
	if (_rollingAction)
	{
		runAction(_rollingAction);
	}
}

bool Player::SetStartPosition(cocos2d::TMXLayer* startPosLayer, cocos2d::Vec2 tileSize)
{
	const Size visibleSize = Director::getInstance()->getVisibleSize();
	const Vec2 origin = Director::getInstance()->getVisibleOrigin();
	auto mapSize = startPosLayer->getLayerSize();
	for (int y = 0; y < mapSize.height; y++)
	{
		for (int x = 0; x < mapSize.width; x++)
		{
			auto startPoint = Vec2{ (float)x,(float)y };
			auto startGid = startPosLayer->getTileGIDAt(startPoint);
			if (startGid != 0)
			{
				auto putPos = Vec2(startPoint.x * tileSize.x + (tileSize.x / 2) + origin.x, (mapSize.height - startPoint.y) * tileSize.y - (tileSize.y / 2) + _point.y + origin.y);
				setPosition(putPos);
				return true;
			}
		}
	}
	return false;
}

void Player::SetAction(ACTION action)
{
	for (auto bl : _blackList[action])
	{
		if (_action == bl)
		{
			return;
		}
	}

	_action = action;
}

ACTION Player::GetAction()
{
	return _action;
}

void Player::FlowerRolling(bool flag)
{
	if (_gameOverFlag)
	{
		return;
	}

	_flowerFlag = flag;
	if (flag)
	{
		_action = ACTION::ROLLING;
		_jumpFlag = false;
		_vector = 0.0f;
		_time = 0.0f;
	}
}

bool Player::GetFlowerFlag()
{
	return _flowerFlag;
}

void Player::Jumping()
{
	if (_jumpFlag)
	{
		Vec2 pos = getPosition();
		if (_vector > _maxVec)
		{
			if (_airTime <= 0.0f)
			{
				_vector = 0.0f;
				_time = 0.0f;
				_action = ACTION::ROLLING;
				return;
			}
			_airTime -= 0.1f;
		}
		else
		{
			_vector = (_maxVec * _time / 2.0f);
			setPosition(Vec2(pos.x, pos.y + _vector));
			_time += 0.1f;
		}
	}
}

void Player::Rolling(float delta)
{
	if (getName() != "player_front")
	{
		return;
	}

	auto test = (1.0f / _moveVec) * delta;
	if (!CollsionCheck(Vec2(test + _point.x, _point.y)))
	{
		auto gameScne = Director::getInstance()->getRunningScene();
		for (auto player : gameScne->getChildByName("PLAYER_LAYER")->getChildren())
		{
			((Obj*)player)->DamageAction(this);
		}
		((HPMng*)gameScne->getChildByName("HP"))->DamageHP(1);
	}
}

void Player::Damage()
{
	if ((_damageFlag))
	{
		// ±¸¼®Ý‚ªI‚í‚Á‚Ä‚¢‚é
		if (_damageAction == nullptr)
		{
			_damageFlag = false;
			return;
		}
		// ±¸¼®Ý‚ªI‚í‚Á‚½
		if (_damageAction->isDone())
		{
			setVisible(true);
			CC_SAFE_RELEASE_NULL(_damageAction);
			_damageFlag = false;
			SetImage(ACTION::ROTATE);

			if ((_rollingAction == nullptr) && (getName() == "player_front"))
			{
				_action = ACTION::ROTATE;
				((ActionConvey*)Director::getInstance()->getRunningScene()->getChildByName("actionConvey"))->SetActionConvey(ACTION::ROTATE);
			}
		}
	}
}

bool Player::SetImage(ACTION action)
{
	if (_image.find(action) == _image.end())
	{
		return false;
	}

	if (_color == OBJ_COLOR::OBJ_RED)
	{
		setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(_image[action].first));
	}
	else
	{
		setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(_image[action].second));
	}
	return true;
}
