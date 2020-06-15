#include "CheckPoint.h"

USING_NS_CC;

Obj* CheckPoint::createCheckPoint()
{
	Obj* pRet = new(std::nothrow) CheckPoint();
	if (pRet && pRet->init())
	{
		pRet->setSpriteFrame(Sprite::create("Ornament/checkPoint.png")->getSpriteFrame());

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

CheckPoint::CheckPoint()
{
}

CheckPoint::~CheckPoint()
{
}

bool CheckPoint::init()
{
	if (!Obstacles::init())
	{
		return false;
	}

	return true;
}

void CheckPoint::DamageAction(cocos2d::Sprite* spite)
{
}

void CheckPoint::GameOverAction()
{
}

void CheckPoint::GameClearAction()
{
}

void CheckPoint::HitCheck(cocos2d::Node* players, HPMng* playerHP)
{
}
