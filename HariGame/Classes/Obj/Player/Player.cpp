#include "Player.h"
#include "GameScene.h"
#include <math.h>

USING_NS_CC;

Obj* Player::createPlayer(OBJ_COLOR color,Vec2 point)
{
	Obj* pRet = new(std::nothrow) Player(point);
	if (pRet && pRet->init())
	{
		if (color == OBJ_COLOR::OBJ_RED)
		{
			pRet->setSpriteFrame(Sprite::create("player/player_r_rotate.png")->getSpriteFrame());
		}
		else if (color == OBJ_COLOR::OBJ_GREEN)
		{
			pRet->setSpriteFrame(Sprite::create("player/player_g_rotate.png")->getSpriteFrame());
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

Player::Player(Vec2 point)
{
	_point = point;
	_vector = 0.0f;
	_time = 0.0f;
	_maxVec = 24.0f;
	_jumpFlag = false;
	_damageFlag = false;
	_action = ACTION::NON;

	_blackList[ACTION::JUMP].push_back(ACTION::JUMP);
	_blackList[ACTION::JUMP].push_back(ACTION::JUMPING);
	_blackList[ACTION::JUMP].push_back(ACTION::FALL);

	_damageAction = nullptr;
	_rollingAction = nullptr;
}

Player::~Player()
{
}

void Player::update(float delta)
{
	if (_action == ACTION::ROTATE)
	{
 		Rotate();
		_action = ACTION::ROLLING;
	}

	if (_action == ACTION::ROLLING)
	{
		//if (CollsionCheck(Vec2(0.0f, -9.8f)))
		//{
		//	_vector = 0.0f;
		//	_time = 0.0f;
		//	_action = ACTION::FALL;
		//}
	}

	if (_action == ACTION::JUMP)
	{
		Jump();
	}
	else if (_action == ACTION::JUMPING)
	{
		Jumping();
	}
	else
	{
		Falling();
	}

	// ﾀﾞﾒｰｼﾞ
	if (_damageFlag)
	{
		DamageAction();
	}

	if (_rollingAction != nullptr)
	{
		
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
					MoveBy::create(1.0f, Vec2(144.0f, 0.0f)),
					nullptr)
			)
		);
		CC_SAFE_RETAIN(_rollingAction);
	}
}

void Player::Jump()
{
	if ((!_jumpFlag) && (_vector <= 0.0f))
	{
		_jumpFlag = true;
		_airTime = 0.8f;
		_action = ACTION::JUMPING;
	}
}

void Player::Falling()
{
	_vector = (9.8f * _time) / 2.0f;
	if (CollsionCheck(Vec2(0.0f, -_vector - _point.y)))
	{
		setPosition(Vec2(getPosition().x, getPosition().y - _vector));
		_time += 0.1f;
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
	checkPoint.y = mapSize.height - checkPoint.y;	// 上下反転

	// 落ちた場合
	if (checkPoint.y >= mapSize.height)
	{
		stopAllActions();
		return false;
	}

	auto gid = scaffoldLayer->getTileGIDAt(checkPoint);
	if ((gid == 210)||(gid == 211))
	{
		checkPoint.y = mapSize.height - checkPoint.y;	// 上下反転
		if (_action == ACTION::FALL)
		{
			setPosition(Vec2(getPosition().x, (checkPoint.y + 1) * TileSize.height - _point.y + (TileSize.height / 2.0f)));
			_action = ACTION::ROTATE;
		}
		return false;
	}

	return true;
}

void Player::DamageAction()
{
	if ((!_damageFlag)&&(_damageAction == nullptr))
	{
		stopAllActions();
		CC_SAFE_RELEASE_NULL(_rollingAction);
		auto pos = getPosition();
		_damageAction = runAction(Sequence::create(
										MoveBy::create(0.2f, Vec2( - 192.0f, 0.0f)),
										DelayTime::create(0.1f),
										nullptr
		));
		CC_SAFE_RETAIN(_damageAction);
		_damageFlag = true;
		return;
	}

	// ｱｸｼｮﾝが終わった
	if ((_damageFlag)&&(_damageAction->isDone()))
	{
		CC_SAFE_RELEASE_NULL(_damageAction);
		_damageFlag = false;
		Rotate();
		return;
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
			if (startGid == 215)
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

bool Player::GetDamageFlag()
{
	return _damageFlag;
}

cocos2d::Vec2 Player::getPoint()
{
	return _point;
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
				_action = ACTION::FALL;
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
