#include "Buds.h"
#include "Obj/Player/Player.h"

USING_NS_CC;

Obj* Buds::createBuds(OBJ_COLOR color)
{
	Obj* pRet = new(std::nothrow) Buds();
	if (pRet && pRet->init())
	{
		SpriteFrameCache::getInstance()->addSpriteFrame(Sprite::create("obstacles/Buds/buds_r.png")->getSpriteFrame(), "buds_r");
		SpriteFrameCache::getInstance()->addSpriteFrame(Sprite::create("obstacles/Buds/buds_r_open.png")->getSpriteFrame(), "open_r");
		SpriteFrameCache::getInstance()->addSpriteFrame(Sprite::create("obstacles/Buds/buds_g.png")->getSpriteFrame(), "buds_g");
		SpriteFrameCache::getInstance()->addSpriteFrame(Sprite::create("obstacles/Buds/buds_g_open.png")->getSpriteFrame(), "open_g");
		if (color == OBJ_COLOR::OBJ_RED)
		{
			pRet->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("buds_r"));
		}

		if (color == OBJ_COLOR::OBJ_GREEN)
		{
			pRet->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("buds_g"));
		}
		pRet->setVisible(true);
		pRet->setFlipY(false);

		pRet->setTag(static_cast<int>(color));
		pRet->setAnchorPoint(Point(0.5f, 0.8f));

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

Buds::Buds()
{
	_damageFlag = false;
	_passingFlag = false;
}

Buds::~Buds()
{
}

void Buds::update(float delta)
{
	if (Director::getInstance()->getRunningScene()->getName() != "GameScene")
	{
		return;
	}

	if (_passingFlag)
	{
		return;
	}

	if (_damageFlag)
	{
		// player‚ð‰Ô‚Ìã‚ð•à‚Ü‚¹‚é
		for (auto player : Director::getInstance()->getRunningScene()->getChildByName("PLAYER_LAYER")->getChildren())
		{
			if ((player->getPositionX() >= this->getPositionX() - 120) && (player->getPositionX() <= this->getPositionX() + 120))
			{
				if ((player->getPositionY() <= this->getPositionY()))
				{
					((Player*)player)->FlowerRolling(true);
					player->setPositionY(getPositionY() + ((Obj*)player)->GetPoint().y);
				}
			}
			else
			{
				((Player*)player)->FlowerRolling(false);
			}

			if ((player->getName()=="player_behind")&&(player->getPositionX() > this->getPositionX() + 120))
			{
				_passingFlag = true;
			}
		}

	}
}

void Buds::DamageAction(cocos2d::Sprite* spite)
{
	// ‰Ô‚ªŠJ‚­
	if (!_damageFlag)
	{
		if (this->getTag() == static_cast<int>(OBJ_COLOR::OBJ_RED))
		{
			this->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("open_r"));
		}
		else if (this->getTag() == static_cast<int>(OBJ_COLOR::OBJ_GREEN))
		{
			this->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("open_g"));
		}
		_damageFlag = true;
	}
}

void Buds::GameOverAction()
{
}

void Buds::GameClearAction()
{
}

void Buds::HitCheck(cocos2d::Node* players, HPMng* playerHP)
{
	// ‰Ô‚ªŠJ‚¢‚Ä‚¢‚é‚Ì‚È‚çˆ—‚µ‚È‚­‚Ä‚æ‚¢
	auto _player_front = (Obj*)players->getChildByName("player_front");

	if (Check(_player_front))
	{
		if (_player_front->getTag() == this->getTag())
		{
			DamageAction(this);
		}
	}
}
