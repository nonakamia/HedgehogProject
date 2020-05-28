#include "Player.h"
#include "Scene/GameScene.h"
#include "HP/HPMng.h"
#include "Action/ActionConvey.h"
#include <math.h>

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

	_moveVec = 192.0f;
	_vector = 0.0f;
	_time = 0.0f;
	_maxVec = 24.0f;
	_jumpFlag = false;
	_flowerFlag = false;
	_action = ACTION::NON;

	_blackList[ACTION::JUMP].push_back(ACTION::JUMP);
	_blackList[ACTION::JUMP].push_back(ACTION::JUMPING);
	_blackList[ACTION::JUMP].push_back(ACTION::FALL);

	_blackList[ACTION::FALL].push_back(ACTION::JUMP);
	_blackList[ACTION::FALL].push_back(ACTION::JUMPING);

	_rollingAction = nullptr;
	_damageAction = nullptr;
	_gemeOverAction = nullptr;
}

Player::~Player()
{
}

void Player::update(float delta)
{
	if (Director::getInstance()->getRunningScene()->getName() != "GameScene")
	{
		return;
	}

	if (!_gameOverFlag)
	{
		if (_action == ACTION::ROTATE)
		{
			Rotate();
			_action = ACTION::ROLLING;
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

		Falling();


		// ÀÞÒ°¼Þ
		if ((_damageFlag))
		{
			DamageAction();
		}
	}
	else
	{
		if (_gemeOverAction->isDone())
		{
			CC_SAFE_RELEASE_NULL(_gemeOverAction);
			unscheduleUpdate();
		}
	}

}

void Player::Rotate()
{
	_rollingAction = runAction(
		RepeatForever::create(
			Spawn::create(
				RotateBy::create(1.0f, 360.0f),
				MoveBy::create(1.0f, Vec2(_moveVec, 0.0f)),
				nullptr)
		)
	);
	CC_SAFE_RETAIN(_rollingAction);
}

void Player::Jump()
{
	if ((!_jumpFlag) && (_vector <= 0.0f))
	{
		_jumpFlag = true;
		_flowerFlag = false;
		_airTime = 0.8f;
		_action = ACTION::JUMPING;

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
		return;
	}

	_vector = (9.8f * _time) / 2.0f;
	if (CollsionCheck(Vec2(0.0f, -_vector - _point.y)))
	{
		setPosition(Vec2(getPosition().x, getPosition().y - _vector));
		_time += 0.1f;
		//_action = ACTION::FALL;
	}
	else
	{
		_jumpFlag = false;
		_time = 0.0f;
		_vector = 0.0f;
	}
}

void Player::Change(int color)
{
	if (color == static_cast<int>(OBJ_COLOR::OBJ_RED))
	{
		setSpriteFrame(Sprite::create("player/player_r_rotate.png")->getSpriteFrame());
		setTag(static_cast<int>(OBJ_COLOR::OBJ_RED));
	}
	else if (color == static_cast<int>(OBJ_COLOR::OBJ_GREEN))
	{
		setSpriteFrame(Sprite::create("player/player_g_rotate.png")->getSpriteFrame());
		setTag(static_cast<int>(OBJ_COLOR::OBJ_GREEN));
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
			setPositionX((checkPoint.x - 1) * TileSize.width + _point.x);
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

void Player::DamageAction()
{
	if ((_action != ACTION::DAMAGE)&&(!_damageFlag)&&(_damageAction == nullptr))
	{
		stopAllActions();
		CC_SAFE_RELEASE_NULL(_rollingAction);

		float time = 0.3f;
		if (getName() == "player_front")
		{
			auto behindPosX = Director::getInstance()->getRunningScene()->getChildByName("PLAYER_LAYER")->getChildByName("player_behind")->getPositionX();
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

		((ActionConvey*)Director::getInstance()->getRunningScene()->getChildByName("actionConvey"))->ConveyClear();

		CC_SAFE_RETAIN(_damageAction);
		_action = ACTION::DAMAGE;
		_damageFlag = true;
		return;
	}

	// ±¸¼®Ý‚ªI‚í‚Á‚Ä‚¢‚é
	if (_damageAction == nullptr)
	{
		_damageFlag = false;
		return;
	}

	// ±¸¼®Ý‚ªI‚í‚Á‚½
	if ((_damageFlag)&&(_damageAction->isDone()))
	{
		CC_SAFE_RELEASE_NULL(_damageAction);
		_damageFlag = false;
		_jumpFlag = false;
		if (getName() == "player_front")
		{
			_action = ACTION::ROTATE;
			((ActionConvey*)Director::getInstance()->getRunningScene()->getChildByName("actionConvey"))->SetActionConvey(ACTION::ROTATE);
		}
		//Rotate();
		return;
	}
}

void Player::GameOverAction()
{
	// ¡‚Ì±¸¼®Ý‚ðŽ~‚ß‚é
	stopAllActions();
	CC_SAFE_RELEASE_NULL(_damageAction);
	CC_SAFE_RELEASE_NULL(_rollingAction);

	// ¹Þ°Ñµ°ÊÞ°Žž‚Ì±¸¼®Ý‚ðs‚¤
	_gemeOverAction = runAction(FadeOut::create(1.0f));
	CC_SAFE_RETAIN(_gemeOverAction);
	_gameOverFlag = true;
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
	_flowerFlag = flag;
	_action = ACTION::ROLLING;

	_jumpFlag = false;
	_vector = 0.0f;
	_time = 0.0f;
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
			_vector = _maxVec * _time / 2.0f;
			setPosition(Vec2(pos.x, pos.y + _vector));
			_time += 0.1f;
		}
	}
}

void Player::Rolling(float delta)
{
	if ((_damageFlag)||(getName() != "player_front"))
	{
		return;
	}
	auto test = (1.0f / _moveVec) * delta;
	if (!CollsionCheck(Vec2(test + _point.x, _point.y)))
	{
		auto gameScne = Director::getInstance()->getRunningScene();
		for (auto player : gameScne->getChildByName("PLAYER_LAYER")->getChildren())
		{
			((Obj*)player)->DamageAction();
		}
		((HPMng*)gameScne->getChildByName("HP"))->DamageHP(1);
	}
}

void Player::SetImage(ACTION action)
{
	if (action == ACTION::JUMP)
	{
		if (getTag() == static_cast<int>(OBJ_COLOR::OBJ_RED))
		{

		}
	}
}
