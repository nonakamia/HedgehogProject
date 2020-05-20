#include "ButtonLayer.h"
#include "GameScene.h"
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

	// ¼Þ¬ÝÌßÎÞÀÝ
	auto jumpButton = MenuItemImage::create("button/button_base.png", "button/button_on_base.png", [](Ref* ref) {});
	jumpButton->setPosition(Vec2(visibleSize.width * 0.9f + origin.x, visibleSize.height * 0.2f + origin.y));
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
			auto player_front = static_cast<Player*>(Director::getInstance()->getRunningScene()->getChildByName("OBJ_LAYER")->getChildByName("player_front"));
			player_front->SetAction(ACTION::JUMP);
			((GameScene*)Director::getInstance()->getRunningScene())->SetActionConvey(ACTION::JUMP);
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
	auto changeButton = MenuItemImage::create("button/button_base.png", "button/button_on_base.png", [](Ref* ref) {});
	changeButton->setPosition(Vec2(visibleSize.width * 0.1f + origin.x, visibleSize.height * 0.2f + origin.y));
	changeButton->setOpacity(150.0f);

	auto changeMenu = Menu::create(changeButton, nullptr);
	changeMenu->setPosition(Point::ZERO);
	this->addChild(changeMenu);

	auto changeListener = EventListenerTouchOneByOne::create();
	// ‰Ÿ‚³‚ê‚½Žž‚Ìˆ—
	changeListener->onTouchBegan = [this, visibleSize, changeButton](Touch* touch, Event* event)
	{
		Vec2 point = touch->getLocation();
		Rect rectButton = changeButton->getBoundingBox();
		if (rectButton.containsPoint(point))
		{

			return true;
		}
		else
		{
			return false;
		}
	};
	// —£‚³‚ê‚½Žž‚Ìˆ—
	changeListener->onTouchEnded = [this](Touch* touch, Event* event)
	{
		auto players = Director::getInstance()->getRunningScene()->getChildByName("OBJ_LAYER");
		auto playerColor = players->getChildByName("player_front")->getTag();
		static_cast<Player*>(players->getChildByName("player_front"))->Change(players->getChildByName("player_behind")->getTag());
		static_cast<Player*>(players->getChildByName("player_behind"))->Change(playerColor);
	};
	director->getEventDispatcher()->addEventListenerWithSceneGraphPriority(changeListener, changeButton);

	// ÃÞÊÞ¯¸—p
#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
	auto listener = EventListenerKeyboard::create();
	listener->onKeyPressed = [this](cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event)->bool
	{
		if (keyCode == cocos2d::EventKeyboard::KeyCode::KEY_SPACE)
		{

			auto player_front = static_cast<Player*>(Director::getInstance()->getRunningScene()->getChildByName("OBJ_LAYER")->getChildByName("player_front"));
			player_front->SetAction(ACTION::JUMP);
			((GameScene*)Director::getInstance()->getRunningScene())->SetActionConvey(ACTION::JUMP);
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
