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
			action = ACTION::ROLLING;
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
	action = ACTION::ROLLING;
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
