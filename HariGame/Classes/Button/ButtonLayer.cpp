#include "ButtonLayer.h"
#include "Scene/GameScene.h"
#include "Obj/Player/Player.h"
#include "Action/ACTION.h"


USING_NS_CC;

Layer* ButtonLayer::createButtonLayer()
{
	ButtonLayer* pRet = new(std::nothrow) ButtonLayer();
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

ButtonLayer::ButtonLayer()
{
}

ButtonLayer::~ButtonLayer()
{
}

static void problemLoading(const char* filename)
{
	printf("Error while loading: %s\n", filename);
	printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in ButtonLayer.cpp\n");
}

bool ButtonLayer::init()
{
	if (!Layer::init())
	{
		return false;
	}
	setName("ButtonLayer");

	const Size visibleSize = Director::getInstance()->getVisibleSize();
	const Vec2 origin = Director::getInstance()->getVisibleOrigin();
	auto director = Director::getInstance();

	_actionConvey = (ActionConvey*)Director::getInstance()->getRunningScene()->getChildByName("actionConvey");

	// ¼Þ¬ÝÌßÎÞÀÝ
	auto jumpButton = MenuItemImage::create("button/button_base.png", "button/button_on_base.png", [](Ref* ref) {});
	jumpButton->setPosition(Vec2(visibleSize.width * 0.9f + origin.x, visibleSize.height * 0.3f + origin.y));
	jumpButton->setOpacity(150.0f);

	auto jumpMenu = Menu::create(jumpButton, nullptr);
	jumpMenu->setPosition(Point::ZERO);
	this->addChild(jumpMenu);

	auto jumpListener = EventListenerTouchOneByOne::create();
	// ‰Ÿ‚³‚ê‚½Žž‚Ìˆ—
	jumpListener->onTouchBegan = [this, visibleSize, jumpButton](Touch* touch, Event* event)
	{
		Vec2 point = touch->getLocation();
		Rect rectButton = jumpButton->getBoundingBox();
		if (rectButton.containsPoint(point))
		{
			auto player_front = static_cast<Player*>(Director::getInstance()->getRunningScene()->getChildByName("PLAYER_LAYER")->getChildByName("player_front"));
			player_front->SetAction(ACTION::JUMP);
			_actionConvey->SetActionConvey(ACTION::JUMP);
			return true;
		}
		else
		{
			return false;
		}
	};
	// —£‚³‚ê‚½Žž‚Ìˆ—
	jumpListener->onTouchEnded = [this](Touch* touch, Event* event)
	{

	};
	director->getEventDispatcher()->addEventListenerWithSceneGraphPriority(jumpListener, jumpButton);

	// “ü‚ê‘Ö‚¦ÎÞÀÝ
	auto changeButton_r = MenuItemImage::create("button/changeButton_r.png", "button/changeButton_r_push.png", [](Ref* ref) {});
	changeButton_r->setPosition(Vec2(visibleSize.width * 0.1f + origin.x, visibleSize.height * 0.3f + origin.y));
	changeButton_r->setOpacity(200.0f);
	changeButton_r->setScale(0.19f);
	//changeButton_r->setVisible(true);

	auto changeButton_g = MenuItemImage::create("button/changeButton_g.png", "button/changeButton_g_push.png", [](Ref* ref) {});
	changeButton_g->setPosition(Vec2(visibleSize.width * 0.1f + origin.x, visibleSize.height * 0.3f + origin.y));
	changeButton_g->setOpacity(200.0f);
	changeButton_g->setScale(0.19f);
	changeButton_g->setVisible(false);


	auto changeMenu_r = Menu::create(changeButton_r, nullptr);
	changeMenu_r->setPosition(Point::ZERO);
	this->addChild(changeMenu_r);

	auto changeMenu_g = Menu::create(changeButton_g, nullptr);
	changeMenu_g->setPosition(Point::ZERO);
	this->addChild(changeMenu_g);

	auto changeListener = EventListenerTouchOneByOne::create();
	// ‰Ÿ‚³‚ê‚½Žž‚Ìˆ—
	changeListener->onTouchBegan = [this, visibleSize, changeButton_r, changeButton_g](Touch* touch, Event* event)->bool
	{
		Vec2 point = touch->getLocation();
		Rect rectButton_r = changeButton_r->getBoundingBox();
		Rect rectButton_g = changeButton_g->getBoundingBox();
		if ((rectButton_r.containsPoint(point)) || (rectButton_g.containsPoint(point)))
		{

			return true;
		}
		else
		{
			return false;
		}
	};
	// —£‚³‚ê‚½Žž‚Ìˆ—
	changeListener->onTouchEnded = [this, changeButton_r, changeButton_g](Touch* touch, Event* event)->bool
	{
		auto players = Director::getInstance()->getRunningScene()->getChildByName("PLAYER_LAYER");
		auto playerColor = players->getChildByName("player_front")->getTag();
		static_cast<Player*>(players->getChildByName("player_front"))->Change(players->getChildByName("player_behind")->getTag());
		static_cast<Player*>(players->getChildByName("player_behind"))->Change(playerColor);

		if (playerColor == static_cast<int>(OBJ_COLOR::OBJ_RED))
		{
			changeButton_r->setVisible(false);
			changeButton_g->setVisible(true);
		}
		else if (playerColor == static_cast<int>(OBJ_COLOR::OBJ_GREEN))
		{
			changeButton_r->setVisible(true);
			changeButton_g->setVisible(false);
		}
		return true;

	};
	director->getEventDispatcher()->addEventListenerWithSceneGraphPriority(changeListener, changeButton_g);

	// ÃÞÊÞ¯¸—p
#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
	auto listener = EventListenerKeyboard::create();
	listener->onKeyPressed = [this](cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event)->bool
	{
		if (keyCode == cocos2d::EventKeyboard::KeyCode::KEY_SPACE)
		{

			auto player_front = static_cast<Player*>(Director::getInstance()->getRunningScene()->getChildByName("PLAYER_LAYER")->getChildByName("player_front"));
			player_front->SetAction(ACTION::JUMP);
			_actionConvey->SetActionConvey(ACTION::JUMP);
		}
		return true;
	};
	listener->onKeyReleased = [this](cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event)->bool
	{
		return true;
	};
	this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
#endif

	return true;
}
