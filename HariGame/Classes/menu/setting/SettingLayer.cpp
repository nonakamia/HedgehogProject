#include <stdlib.h>
#include "SettingLayer.h"
#include "menu/MenuLayer.h"
#include "Split/Split.h"

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
	_maxHP = 0;
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

	// 外部ﾃﾞｰﾀ読み込み
	std::string defaultIfs = FileUtils::getInstance()->getStringFromFile("csv/setting_default.csv");
	if (defaultIfs == "")
	{
		return false;
	}
	ValueVector defaultCsvSplit = Split::split(defaultIfs, "\n");
	for (int i = 1; i < (int)defaultCsvSplit.size(); i++)
	{
		ValueVector defaultCsvData = Split::split(defaultCsvSplit.at(i).asString(), ",");
		_defaultHP = defaultCsvData.at(0).asInt();
	}

	// 以前のﾃﾞｰﾀの呼び出し
	UserDefault* _userDef = UserDefault::getInstance();
	if (_userDef->getIntegerForKey("MAX_HP") != 0)
	{
		_maxHP = _userDef->getIntegerForKey("MAX_HP");
	}
	else
	{
		_maxHP = _defaultHP;
	}

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto menuImag = Sprite::create("menu/menuImage.png");
	addChild(menuImag);
	menuImag->setPosition(Vec2(
		origin.x + visibleSize.width / 2.0f,
		origin.y + visibleSize.height / 2.0f
	));

	auto setting = Sprite::create("menu/setting.png");
	addChild(setting);
	setting->setPosition(Vec2(
		menuImag->getPosition().x,
		menuImag->getPosition().y * 1.5f
	));

	if (Director::getInstance()->getRunningScene()->getName() != "GameScene")
	{
		// 最大HP
		auto maxHP = Sprite::create("Setting/maxHP.png");
		addChild(maxHP);
		maxHP->setPosition(Vec2(
			menuImag->getPosition().x - menuImag->getContentSize().width / 3.0f,
			menuImag->getPosition().y * 1.3f
		));

		_hpLabel = Label::createWithTTF(std::to_string(_maxHP), "fonts/arial.ttf", 50.0f);
		addChild(_hpLabel);
		_hpLabel->setPosition(Vec2(menuImag->getPosition().x + menuImag->getContentSize().width / 4.0f,
			maxHP->getPosition().y));
		_hpLabel->setColor(cocos2d::Color3B(0.0f, 0.0f, 0.0f));

		auto HP_right = MenuItemImage::create(
			"Setting/right.png",
			"Setting/right.png",
			CC_CALLBACK_1(SettingLayer::HP, this, 1));
		HP_right->setAnchorPoint(Point(0.0f, 0.5f));
		HP_right->setPosition(Vec2(
			_hpLabel->getPosition().x + 40.0f,
			maxHP->getPosition().y
		));
		auto HP_rightMenu = Menu::create(HP_right, nullptr);
		HP_rightMenu->setPosition(Vec2::ZERO);
		addChild(HP_rightMenu);

		auto HP_left = MenuItemImage::create(
			"Setting/left.png",
			"Setting/left.png",
			CC_CALLBACK_1(SettingLayer::HP, this, -1));
		HP_left->setAnchorPoint(Point(1.0f, 0.5f));
		HP_left->setPosition(Vec2(
			_hpLabel->getPosition().x - 40.0f,
			maxHP->getPosition().y
		));
		auto HP_leftMenu = Menu::create(HP_left, nullptr);
		HP_leftMenu->setPosition(Vec2::ZERO);
		addChild(HP_leftMenu);
	}
	// ﾘｾｯﾄ
	auto reset = MenuItemImage::create(
		"Setting/reset.png",
		"Setting/reset.png",
		CC_CALLBACK_1(SettingLayer::Reset, this));
	reset->setPosition(Vec2(
		menuImag->getPosition().x,
		menuImag->getPosition().y - menuImag->getContentSize().height / 4.0f
	));
	auto resetMenu = Menu::create(reset, nullptr);
	resetMenu->setPosition(Vec2::ZERO);
	addChild(resetMenu);

	// ﾒﾆｭｰに戻る
	auto backM= MenuItemImage::create(
		"Setting/backMenu.png",
		"Setting/backMenu.png",
		CC_CALLBACK_1(SettingLayer::BackMenu, this));
	backM->setPosition(Vec2(
		menuImag->getPosition().x,
		menuImag->getPosition().y * 0.5f
	));
	auto backMMenu = Menu::create(backM, nullptr);
	backMMenu->setPosition(Vec2::ZERO);
	addChild(backMMenu);

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

	// 設定の確定
	UserDefault* _userDef = UserDefault::getInstance();
	_userDef->setIntegerForKey("MAX_HP", _maxHP);
	
	Size visibleSize = Director::getInstance()->getVisibleSize();
	_backAction = this->runAction(
		MoveTo::create(0.5f, Vec2(visibleSize.width,this->getPositionY())));
	CC_SAFE_RETAIN(_backAction);

	auto menuLayer = (MenuLayer*)Director::getInstance()->getRunningScene()->getChildByName("MenuLayer");
	menuLayer->Resume();
}

void SettingLayer::HP(Ref* pSender, int addHP)
{
	if (Director::getInstance()->getRunningScene()->getName() == "GameScene")
	{
		return;
	}
	if ((_maxHP + addHP < 1) || (_maxHP + addHP > 5))
	{
		return;
	}

	_maxHP += addHP;
	_hpLabel->setString(std::to_string(_maxHP));
}

void SettingLayer::Reset(Ref* pSender)
{
	if (Director::getInstance()->getRunningScene()->getName() != "GameScene")
	{
		_maxHP = _defaultHP;
		_hpLabel->setString(std::to_string(_maxHP));
	}
}
