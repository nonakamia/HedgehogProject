#include "Buds.h"
#include "Obj/Player/Player.h"

USING_NS_CC;

Obj* Buds::createBuds(OBJ_COLOR color)
{
	Obj* pRet = new(std::nothrow) Buds();
	if (pRet && pRet->init())
	{
		if (color == OBJ_COLOR::OBJ_RED)
		{
			SpriteFrameCache::getInstance()->addSpriteFrame(Sprite::create("obstacles/Buds/buds_r.png")->getSpriteFrame(), "buds");
			SpriteFrameCache::getInstance()->addSpriteFrame(Sprite::create("obstacles/Buds/buds_r_open.png")->getSpriteFrame(), "open");
		}
		else if (color == OBJ_COLOR::OBJ_GREEN)
		{
			SpriteFrameCache::getInstance()->addSpriteFrame(Sprite::create("obstacles/Buds/buds_g.png")->getSpriteFrame(), "buds");
			SpriteFrameCache::getInstance()->addSpriteFrame(Sprite::create("obstacles/Buds/buds_g_open.png")->getSpriteFrame(), "open");
		}
		pRet->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("buds"));
		pRet->setTag(static_cast<int>(color));
		pRet->setAnchorPoint(Point(0.5f, 0.8f));
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
	_openFlag = false;
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

	if (_openFlag)
	{
		// player‚ð‰Ô‚Ìã‚ð•à‚Ü‚¹‚é
		for (auto player : Director::getInstance()->getRunningScene()->getChildByName("PLAYER_LAYER")->getChildren())
		{
			if (((Player*)player)->GetFlowerFlag())
			{
				player->setPositionY(getPositionY() + ((Obj*)player)->GetPoint().y);
			}
		}

	}
}

void Buds::DamageAction()
{
	// ‰Ô‚ªŠJ‚­
	setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("open"));
	_openFlag = true;
	for (auto player : Director::getInstance()->getRunningScene()->getChildByName("PLAYER_LAYER")->getChildren())
	{
		((Player*)player)->FlowerRolling(true);
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
	if (_openFlag)
	{
		return;
	}

	auto _player_front = (Obj*)players->getChildByName("player_front");

	if (Check(_player_front))
	{
		if (_player_front->getTag() == this->getTag())
		{
			DamageAction();
		}
	}
}
