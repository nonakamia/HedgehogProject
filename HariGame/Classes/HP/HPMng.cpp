#include "HPMng.h"

USING_NS_CC;

HPMng* HPMng::createHPMng(int maxHP)
{
	HPMng* pRet = new(std::nothrow)HPMng(maxHP);
	if (pRet && pRet->init())
	{
		//SpriteFrameCache::getInstance()->addSpriteFrame(Sprite::create("HP/heart.png")->getSpriteFrame(), "heart");

		return pRet;
	}
	else
	{
		delete pRet;
		pRet = nullptr;
		return nullptr;
	}
}

HPMng::HPMng(int maxHP)
{
	_maxHP = maxHP;
	_hP = maxHP;
}

HPMng::~HPMng()
{
}

bool HPMng::init()
{
	if (!Layer::init())
	{
		return false;
	}
	setName("HPMng");

	const Size visibleSize = Director::getInstance()->getVisibleSize();
	const Vec2 origin = Director::getInstance()->getVisibleOrigin();

	for (int hp = 0; hp < _maxHP; hp++)
	{
		auto heart = Sprite::create("HP/heart.png");
		auto name = std::string("heart_");
		name.append(std::to_string(hp));
		heart->setName(name);
		addChild(heart);
		heart->setPosition(Vec2(visibleSize.width - heart->getContentSize().width - ((heart->getContentSize().width+10) * hp), visibleSize.height - heart->getContentSize().height));
	}

	return true;
}

void HPMng::DamageHP(int damage)
{
	if (_hP <= 0)
	{
		return;
	}

	for (int hp = 0; hp < damage; hp++)
	{
		auto name = std::string("heart_");
		name.append(std::to_string(_hP - 1));
		auto heart = getChildByName(name);
		heart->setVisible(false);
		_hP--;
	}
}

int HPMng::GetHP()
{
	return _hP;
}