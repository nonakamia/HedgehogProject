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

	//@cricket
	_openSE = nullptr;
}

Buds::~Buds()
{
	//@cricket
	if (_openSE)
	{
		_openSE->destroy();
		_openSE = nullptr;
	}
}

bool Buds::init()
{
	if (!Obstacles::init())
	{
		return false;
	}

	//@cricket
	_openSE = CkSound::newBankSound(_obstaclesBank, "budsOpen");
	
	return true;
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
		// playerを花の上を歩ませる
		for (auto player : Director::getInstance()->getRunningScene()->getChildByName("PLAYER_LAYER")->getChildren())
		{
			if ((player->getPosition().x >= this->getPosition().x - 120) && (player->getPosition().x <= this->getPosition().x + 120))
			{
				if ((player->getPosition().y <= this->getPosition().y))
				{
					((Player*)player)->FlowerRolling(true);
					player->setPosition(player->getPosition().x, getPosition().y + ((Obj*)player)->GetPoint().y);

				}
			}
			else
			{
				((Player*)player)->FlowerRolling(false);
			}

			if ((player->getName()=="player_behind")&&(player->getPosition().x > this->getPosition().x + 120))
			{
				_passingFlag = true;
			}
		}

	}
}

void Buds::DamageAction(cocos2d::Sprite* spite)
{
	// 花が開く
	if (!_damageFlag)
	{
		_openSE->play();
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
	// 花が開いているのなら処理しなくてよい
	auto _player_front = (Obj*)players->getChildByName("player_front");

	if (Check(_player_front))
	{
		if (_player_front->getTag() == this->getTag())
		{
			DamageAction(this);
		}
	}
}
