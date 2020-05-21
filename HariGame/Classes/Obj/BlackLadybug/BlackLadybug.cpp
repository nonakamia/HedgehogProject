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
		pRet->setAnchorPoint(Point(0.5f, 0.5f));
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
}

BlackLadybug::~BlackLadybug()
{
}

void BlackLadybug::DamageAction()
{
	if (!_damageFlag)
	{
		setVisible(false);
		_damageFlag = true;
	}
}

void BlackLadybug::GameOverAction()
{
}

void BlackLadybug::HitCheck(cocos2d::Node* players, HPMng* playerHP)
{
	auto _player_front = (Obj*)players->getChildByName("player_front");

	if (_player_front->GetDamageFlag())
	{
		return;
	}

	if ((_player_front->getPosition().x - _player_front->GetPoint().x <= this->getPosition().x + this->GetPoint().x) &&
		(_player_front->getPosition().x + _player_front->GetPoint().x >= this->getPosition().x - this->GetPoint().x) &&
		(_player_front->getPosition().y - _player_front->GetPoint().y <= this->getPosition().y + this->GetPoint().y) &&
		(_player_front->getPosition().y + _player_front->GetPoint().y >= this->getPosition().y - this->GetPoint().y))
	{
		if (_player_front->getTag() == this->getTag())
		{
			DamageAction();
		}
		else if (!_player_front->GetDamageFlag())
		{
			for (auto player : players->getChildren())
			{
				((Obj*)player)->DamageAction();
			}
			DamageAction();
			playerHP->DamageHP(1);
		}
	}
}
