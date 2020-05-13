#include "Player.h"

USING_NS_CC;

Obj* Player::createPlayer()
{
	Obj* pRet = new(std::nothrow) Player();
	if (pRet && pRet->init())
	{
		pRet->setSpriteFrame(Sprite::create("player/player_r_rotate.png")->getSpriteFrame());
		pRet->setScale(0.2f);
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
	// ÃÞÊÞ¯¸—p
#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
	auto listener = EventListenerKeyboard::create();
	listener->onKeyPressed = [this](cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event)->bool
	{
		if (keyCode == cocos2d::EventKeyboard::KeyCode::KEY_SPACE)
		{
			PlayerRolling();
			_action = ACTION::ROLLING;
		}
		if (keyCode == cocos2d::EventKeyboard::KeyCode::KEY_UP_ARROW)
		{
			if (CollsionCheck(cocos2d::Vec2(0.0f, 24.0f)))
			{
				this->setPosition(cocos2d::Vec2(this->getPosition().x, this->getPosition().y + 24.0f));
			}
		}
		if (keyCode == cocos2d::EventKeyboard::KeyCode::KEY_DOWN_ARROW)
		{
			if (CollsionCheck(cocos2d::Vec2(0.0f, -24.0f)))
			{
				this->setPosition(cocos2d::Vec2(this->getPosition().x, this->getPosition().y - 24.0f));
			}
		}
		if (keyCode == cocos2d::EventKeyboard::KeyCode::KEY_RIGHT_ARROW)
		{
			if (CollsionCheck(cocos2d::Vec2(24.0f, 0.0f)))
			{
				this->setPosition(cocos2d::Vec2(this->getPosition().x + 24.0f, this->getPosition().y));
			}
		}
		if (keyCode == cocos2d::EventKeyboard::KeyCode::KEY_LEFT_ARROW)
		{
			if (CollsionCheck(cocos2d::Vec2(-24.0f, 0.0f)))
			{
				this->setPosition(cocos2d::Vec2(this->getPosition().x - 24.0f, this->getPosition().y));
			}
		}
		return true;
	};
	listener->onKeyReleased = [this](cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event)->bool
	{
		if (keyCode == cocos2d::EventKeyboard::KeyCode::KEY_SPACE)
		{
			stopAllActions();
		}
		return true;
	};
	this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
#else
	PlayerRolling();
	_action = ACTION::ROLLING;
#endif

}

Player::~Player()
{
}

void Player::PlayerRolling()
{
	this->runAction(
		RepeatForever::create(
			RotateBy::create(1.0f, 360.0f)
		)
	);
}

bool Player::CollsionCheck(cocos2d::Vec2 vec)
{
	cocos2d::TMXTiledMap* mapData = (cocos2d::TMXTiledMap*)Director::getInstance()->getRunningScene()->getChildByName("BG_LAYER")->getChildByName("MapData");
	cocos2d::TMXLayer* scaffoldLayer = mapData->getLayer("scaffold");

	if (!scaffoldLayer)
	{
		return false;
	}

	auto mapSize = scaffoldLayer->getLayerSize();

	auto checkPoint = Vec2{ this->getPosition().x + vec.x,this->getPosition().y + vec.y }/48;
	checkPoint.y = mapSize.height - checkPoint.y;	// ã‰º”½“]
	auto gid = scaffoldLayer->getTileGIDAt(checkPoint);
	if ((gid == 210)||(gid==211))
	{
		return false;
	}

	return true;
}
