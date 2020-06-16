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
	_damageFlag = false;

	//@cricket
	_checkPointSE = nullptr;
}

CheckPoint::~CheckPoint()
{
	//@cricket
	if (_checkPointSE)
	{
		_checkPointSE->destroy();
		_checkPointSE = nullptr;
	}
}

bool CheckPoint::init()
{
	if (!Obstacles::init())
	{
		return false;
	}

	//@cricket
	_checkPointSE = CkSound::newBankSound(_obstaclesBank, "checkPoint");
	return true;
}

void CheckPoint::DamageAction(cocos2d::Sprite* spite)
{
	if (_damageFlag)
	{
		return;
	}

	//@cricket
	_checkPointSE->play();

	// Áª¯¸Îß²ÝÄ‚ÌÀ•W‹L˜^
	UserDefault* _userDef = UserDefault::getInstance();
	_userDef->setFloatForKey("C_POINT_X", this->getPosition().x);
	_userDef->setFloatForKey("C_POINT_Y", this->getPosition().y + this->getContentSize().height);
	
	if (spite->getTag() == static_cast<int>(OBJ_COLOR::OBJ_RED))
	{
		this->setColor(Color3B(255.0f, 99.0f, 110.0f));
	}
	else
	if (spite->getTag() == static_cast<int>(OBJ_COLOR::OBJ_GREEN))
	{
		this->setColor(Color3B(168.0f, 255.0f, 101.0f));
	}


	_damageFlag = true;
}

void CheckPoint::GameOverAction()
{
}

void CheckPoint::GameClearAction()
{
}

void CheckPoint::HitCheck(cocos2d::Node* players, HPMng* playerHP)
{
	auto _player_front = (Obj*)players->getChildByName("player_front");
	if (_player_front->getPositionX() >= this->getPositionX())
	{
		DamageAction(_player_front);
	}
}
