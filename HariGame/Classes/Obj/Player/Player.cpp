#include "Player.h"

USING_NS_CC;

Obj* Player::createPlayer()
{
	Obj* pRet = new(std::nothrow) Player();
	if (pRet && pRet->init())
	{
		pRet->setSpriteFrame(Sprite::create("HelloWorld.png")->getSpriteFrame());
		
		pRet->runAction(
			RepeatForever::create(
				RotateBy::create(1.0f, 360.0f)
			)
		);

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
}

Player::~Player()
{
}
