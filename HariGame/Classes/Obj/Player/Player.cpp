#include "Player.h"
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
	_maxVec = 40.0f;
	_jumpFlag = false;
	_action = ACTION::FALL;

	_blackList[ACTION::JUMP].push_back(ACTION::JUMP);
	_blackList[ACTION::JUMP].push_back(ACTION::JUMPING);
	_blackList[ACTION::JUMP].push_back(ACTION::FALL);

	_rollingAction = nullptr;

	// ÃÞÊÞ¯¸—p
#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
	auto listener = EventListenerKeyboard::create();
	listener->onKeyPressed = [this](cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event)->bool
	{
		if (keyCode == cocos2d::EventKeyboard::KeyCode::KEY_SPACE)
		{

			_action = ACTION::JUMP;
		}
		if (keyCode == cocos2d::EventKeyboard::KeyCode::KEY_UP_ARROW)
		{
			if (CollsionCheck(cocos2d::Vec2(_point.x, 24.0f + _point.y)))
			{
				this->setPosition(cocos2d::Vec2(this->getPosition().x, this->getPosition().y + 12.0f));
			}
		}
		if (keyCode == cocos2d::EventKeyboard::KeyCode::KEY_DOWN_ARROW)
		{
			if (CollsionCheck(cocos2d::Vec2(_point.x, -24.0f - _point.y)))
			{
				this->setPosition(cocos2d::Vec2(this->getPosition().x, this->getPosition().y - 12.0f));
			}
		}
		if (keyCode == cocos2d::EventKeyboard::KeyCode::KEY_RIGHT_ARROW)
		{
			if ((CollsionCheck(cocos2d::Vec2(24.0f + _point.x, _point.y)))&& (CollsionCheck(cocos2d::Vec2(24.0f + _point.x, -_point.y))))
			{
				this->setPosition(cocos2d::Vec2(this->getPosition().x + 12.0f, this->getPosition().y));
			}
		}
		if (keyCode == cocos2d::EventKeyboard::KeyCode::KEY_LEFT_ARROW)
		{
			if (CollsionCheck(cocos2d::Vec2(-24.0f - _point.x, 0.0f)))
			{
				this->setPosition(cocos2d::Vec2(this->getPosition().x - 12.0f, this->getPosition().y));
			}
		}
		return true;
	};
	listener->onKeyReleased = [this](cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event)->bool
	{
		if (keyCode == cocos2d::EventKeyboard::KeyCode::KEY_SPACE)
		{
			//stopAllActions();
		}
		return true;
	};
	this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
#else
#endif

}

Player::~Player()
{
}

void Player::update(float delta)
{
	if (_action == ACTION::ROLL)
	{
 		PlayerRolling();
		_action = ACTION::ROLLING;
	}
	if (_action == ACTION::ROLLING)
	{

	}
	if (_action == ACTION::JUMP)
	{
		Jump();
	}
	if (_action == ACTION::JUMPING)
	{
		Jumping();
	}
	if (_action == ACTION::FALL)
	{
		Falling();
	}
}

void Player::PlayerRolling()
{
	if (_rollingAction == nullptr)
	{
		_rollingAction = runAction(
			RepeatForever::create(
				Spawn::create(
					RotateBy::create(1.0f, 360.0f),
					MoveBy::create(1.0f, Vec2(96.0f, 0.0f)),
					nullptr)
			)
		);
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
	_vector = (9.8f * _time)/2;
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

	auto checkPoint = Vec2{ this->getPosition().x + vec.x,this->getPosition().y + vec.y }/48;
	checkPoint.y = mapSize.height - checkPoint.y;	// ã‰º”½“]
	auto gid = scaffoldLayer->getTileGIDAt(checkPoint);
	if ((gid == 210)||(gid == 211))
	{
		checkPoint.y = mapSize.height - checkPoint.y;	// ã‰º”½“]
		if (_action == ACTION::FALL)
		{
			setPosition(Vec2(getPosition().x, (checkPoint.y + 1) * 48 - _point.y + 24));
			_action = ACTION::ROLL;
		}
		return false;
	}

	return true;
}

void Player::setAction(ACTION action)
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
			_vector = _maxVec * _time;
			setPosition(Vec2(pos.x, pos.y + _vector));
			_time += 0.1f;
		}
	}
}
