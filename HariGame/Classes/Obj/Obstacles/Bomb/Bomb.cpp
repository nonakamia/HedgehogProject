#include "Bomb.h"

USING_NS_CC;

Obj* Bomb::createBomb(OBJ_COLOR color)
{
	Obj* pRet = new(std::nothrow) Bomb();
	if (pRet && pRet->init())
	{
		if (color == OBJ_COLOR::OBJ_RED)
		{
			pRet->setSpriteFrame(Sprite::create("obstacles/bomb/bomb_r.png")->getSpriteFrame());
		}
		else if (color == OBJ_COLOR::OBJ_GREEN)
		{
			pRet->setSpriteFrame(Sprite::create("obstacles/bomb/bomb_g.png")->getSpriteFrame());
		}

		pRet->setTag(static_cast<int>(color));

		pRet->runAction(RepeatForever::create(
			Sequence::create(
				ResizeTo::create(0.5f, Size(100.0f, 80.0f)),
				ResizeTo::create(0.5f, Size(100.0f, 100.0f)),
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

Bomb::Bomb()
{
	_damageFlag = false;

	//@cricket
	_hitSE = nullptr;
}

Bomb::~Bomb()
{
	//@cricket
	if (_hitSE)
	{
		_hitSE->destroy();
		_hitSE = nullptr;
	}
}

bool Bomb::init()
{
	if (!Obstacles::init())
	{
		return false;
	}

	//@cricket
	_hitSE = CkSound::newBankSound(_obstaclesBank, "bomb");

	return true;
}

void Bomb::update(float delta)
{
}

void Bomb::DamageAction(cocos2d::Sprite* spite)
{
	if (!_damageFlag)
	{
		_hitSE->play();
		setVisible(false);
		_damageFlag = true;
	}
}

void Bomb::GameOverAction()
{
}

void Bomb::GameClearAction()
{
}

void Bomb::HitCheck(cocos2d::Node* players, HPMng* playerHP)
{
	auto _player_front = (Obj*)players->getChildByName("player_front");

	// ÀÞÒ°¼ÞŽó‚¯‚Ä‚¢‚½‚È‚ç–³Ž‹‚·‚é
	if (_player_front->GetDamageFlag())
	{
		return;
	}

	if (Check(_player_front))
	{
		if (_player_front->getTag() == this->getTag())
		{
			if (!_player_front->GetDamageFlag())
			{
				DamageAction(this);
				for (auto player : players->getChildren())
				{
					((Obj*)player)->DamageAction(this);
				}
				playerHP->DamageHP(1);
			}
		}
	}
}
