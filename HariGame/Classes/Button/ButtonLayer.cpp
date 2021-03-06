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

	auto scene = Director::getInstance()->getRunningScene();
	_actionConvey = (ActionConvey*)scene->getChildByName("actionConvey");

	// ｼﾞｬﾝﾌﾟﾎﾞﾀﾝ
	auto jumpButton = MenuItemImage::create("button/jumpButton.png", "button/jumpButton_push.png", [](Ref* ref) {});
	jumpButton->setPosition(Vec2(visibleSize.width * 0.9f + origin.x, visibleSize.height * 0.3f + origin.y));
	jumpButton->setOpacity(150.0f);

	auto jumpMenu = Menu::create(jumpButton, nullptr);
	jumpMenu->setPosition(Point::ZERO);
	this->addChild(jumpMenu);

	auto jumpListener = EventListenerTouchOneByOne::create();
	// 押された時の処理
	jumpListener->onTouchBegan = [this, scene, jumpButton](Touch* touch, Event* event)
	{
		if ((!((GameScene*)scene)->GetMenuFlag()) && ((!((GameScene*)scene)->GetGoalFlag())))
		{
			Vec2 point = touch->getLocation();
			Rect rectButton = jumpButton->getBoundingBox();
			if (rectButton.containsPoint(point))
			{
				auto player_front = static_cast<Player*>(scene->getChildByName("PLAYER_LAYER")->getChildByName("player_front"));
				player_front->SetAction(ACTION::JUMP);
				_actionConvey->SetActionConvey(ACTION::JUMP);
				return true;
			}
			else
			{
				return false;
			}
		}
		return false;
	};
	// 離された時の処理
	jumpListener->onTouchEnded = [this](Touch* touch, Event* event)
	{

	};
	director->getEventDispatcher()->addEventListenerWithSceneGraphPriority(jumpListener, jumpButton);

	// 入れ替えﾎﾞﾀﾝ
	auto changeButton_r = MenuItemImage::create("button/changeButton_r.png", "button/changeButton_r_push.png", [](Ref* ref) {});
	changeButton_r->setPosition(Vec2(visibleSize.width * 0.1f + origin.x, visibleSize.height * 0.3f + origin.y));
	changeButton_r->setOpacity(200.0f);
	//changeButton_r->setVisible(true);

	auto changeButton_g = MenuItemImage::create("button/changeButton_g.png", "button/changeButton_g_push.png", [](Ref* ref) {});
	changeButton_g->setPosition(Vec2(visibleSize.width * 0.1f + origin.x, visibleSize.height * 0.3f + origin.y));
	changeButton_g->setOpacity(200.0f);
	changeButton_g->setVisible(false);


	auto changeMenu_r = Menu::create(changeButton_r, nullptr);
	changeMenu_r->setPosition(Point::ZERO);
	this->addChild(changeMenu_r);

	auto changeMenu_g = Menu::create(changeButton_g, nullptr);
	changeMenu_g->setPosition(Point::ZERO);
	this->addChild(changeMenu_g);

	auto changeListener = EventListenerTouchOneByOne::create();
	// 押された時の処理
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
	// 離された時の処理
	changeListener->onTouchEnded = [this, scene, changeButton_r, changeButton_g](Touch* touch, Event* event)->bool
	{
		if ((!((GameScene*)scene)->GetMenuFlag())&&((!((GameScene*)scene)->GetGoalFlag())))
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
		}
		return true;
	};
	director->getEventDispatcher()->addEventListenerWithSceneGraphPriority(changeListener, changeButton_g);

	// ﾃﾞﾊﾞｯｸ用
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
