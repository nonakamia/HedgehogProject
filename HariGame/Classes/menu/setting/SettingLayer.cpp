#include "SettingLayer.h"
#include "menu/MenuLayer.h"

USING_NS_CC;

Layer* SettingLayer::createSettingLayer()
{
	SettingLayer* pRet = new(std::nothrow) SettingLayer();
	if (pRet && pRet->init())
	{
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

SettingLayer::SettingLayer()
{
	_backAction = nullptr;
}

SettingLayer::~SettingLayer()
{
}

bool SettingLayer::init()
{
	if (!Layer::init())
	{
		return false;
	}
	setName("SettingLayer");

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto menuImag = Sprite::create("menu/menuImage.png");
	addChild(menuImag);
	menuImag->setPosition(Vec2(
		origin.x + visibleSize.width / 2.0f,
		origin.y + visibleSize.height / 2.0f
	));

	auto backMenu= MenuItemImage::create(
		"menu/backMenu.png",
		"menu/backMenu.png",
		CC_CALLBACK_1(SettingLayer::BackMenu, this));
	backMenu->setPosition(Vec2(
		menuImag->getPosition().x,
		menuImag->getPosition().y * 0.5f
	));
	auto backMenuMenu = Menu::create(backMenu, nullptr);
	backMenuMenu->setPosition(Vec2::ZERO);
	addChild(backMenuMenu);

	this->setPositionX(origin.x + visibleSize.width);
	this->runAction(MoveTo::create(1.0f, Vec2::ZERO));

	this->scheduleUpdate();

	return true;
}

void SettingLayer::update(float delta)
{
	if (_backAction == nullptr)
	{
		return;
	}

	if (_backAction->isDone())
	{
		CC_SAFE_RELEASE_NULL(_backAction);
		this->removeFromParentAndCleanup(true);
	}
}

void SettingLayer::BackMenu(Ref* pSender)
{
	if (_backAction != nullptr)
	{
		return;
	}

	Size visibleSize = Director::getInstance()->getVisibleSize();
	_backAction = this->runAction(
		MoveTo::create(0.5f, Vec2(visibleSize.width,this->getPositionY())));
	CC_SAFE_RETAIN(_backAction);

	auto menuLayer = (MenuLayer*)Director::getInstance()->getRunningScene()->getChildByName("MenuLayer");
	menuLayer->Resume();
}
