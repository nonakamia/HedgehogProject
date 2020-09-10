#include "BlackLadybug.h"

USING_NS_CC;

Obj* BlackLadybug::createBlackLadybug(OBJ_COLOR color)
{
	Obj* pRet = new(std::nothrow) BlackLadybug();
	if (pRet && pRet->init())
	{
		if (color == OBJ_COLOR::OBJ_RED)
		{
			pRet->setSpriteFrame(Sprite::create("obstacles/blackLadybug_r.png")->getSpriteFrame());
		}
		else if(color == OBJ_COLOR::OBJ_GREEN)
		{
			pRet->setSpriteFrame(Sprite::create("obstacles/blackLadybug_g.png")->getSpriteFrame());
		}

		pRet->setTag(static_cast<int>(color));
		pRet->setAnchorPoint(Point(0.5f, 0.0f));

		pRet->setScale(1.5f);
		pRet->runAction(RepeatForever::create(
			Sequence::create(
				ResizeTo::create(0.5f, Size(100.0f, 100.0f)),
				ResizeTo::create(0.5f, Size(100.0f, 120.0f)),
				nullptr
			)));

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

BlackLadybug::BlackLadybug()
{
	_damageFlag = false;

	//@cricket
	_hitSE = nullptr;
}

BlackLadybug::~BlackLadybug()
{
	if (_hitSE)
	{
		_hitSE->destroy();
		_hitSE = nullptr;
	}
}

bool BlackLadybug::init()
{
	if (!Obstacles::init())
	{
		return false;
	}
	_hitSE = CkSound::newBankSound(_obstaclesBank, "hit");
	return true;
}

void BlackLadybug::DamageAction(cocos2d::Sprite* spite)
{
	if (!_damageFlag)
	{
		_hitSE->play();
		setVisible(false);
		_damageFlag = true;
	}
}

void BlackLadybug::GameOverAction()
{
}

void BlackLadybug::GameClearAction()
{
}

void BlackLadybug::HitCheck(cocos2d::Node* players, HPMng* playerHP)
{
	auto _player_front = (Obj*)players->getChildByName("player_front");

	// ÀÞÒ°¼ÞŽó‚¯‚Ä‚¢‚½‚È‚ç–³Ž‹‚·‚é
	if (_player_front->GetDamageFlag())
	{
		return;
	}

	if(Check(_player_front))
	{
		if (_player_front->getTag() == this->getTag())
		{
			DamageAction(this);
		}
		else if (!_player_front->GetDamageFlag())
		{
			for (auto player : players->getChildren())
			{
				((Obj*)player)->DamageAction(this);
			}

			playerHP->DamageHP(1);
		}
	}
}
