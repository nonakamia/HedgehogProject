#include "StageLayer.h"
#include "Scene/StageSelectScene.h"

USING_NS_CC;

Layer* StageLayer::createStageLayer(std::string map,cocos2d::Vec2 pos)
{
	StageLayer* pRet = new(std::nothrow) StageLayer(map,pos);
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

StageLayer::StageLayer(std::string map,cocos2d::Vec2 pos)
{
	_map = map;
	_position = pos;
	_calloutFlag = false;
}

StageLayer::~StageLayer()
{
}

bool StageLayer::init()
{
	if (!Layer::init())
	{
		return false;
	}

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto stage = Sprite::create("StageSelect/stage.png");
	addChild(stage);
	stage->setPosition(_position);
	auto changeListener = EventListenerTouchOneByOne::create();
	// ‰Ÿ‚³‚ê‚½Žž‚Ìˆ—
	changeListener->onTouchBegan = [this, visibleSize, stage](Touch* touch, Event* event)
	{
		Vec2 point = touch->getLocation();
		Rect rectButton = stage->getBoundingBox();
		if (rectButton.containsPoint(point))
		{

			return true;
		}
		else
		{
			_callout->runAction(ScaleTo::create(0.1f, 0.0f));
			_calloutFlag = false;
		}
		return false;
	};
	// —£‚³‚ê‚½Žž‚Ìˆ—
	changeListener->onTouchEnded = [this](Touch* touch, Event* event)
	{
		if (!_calloutFlag)
		{
			_callout->runAction(ScaleTo::create(0.1f, 1.0f));
			_calloutFlag = true;
		}
		else
		{
			auto scene = Director::getInstance()->getRunningScene();
			((StageSelectScene*)scene)->changeScene(this, _map);
		}
		return true;
	};
	getEventDispatcher()->addEventListenerWithSceneGraphPriority(changeListener, this);

	// ‚«o‚µ
	_callout = Sprite::create("StageSelect/callout.png");
	_callout->setName("callout");
	addChild(_callout);
	_callout->setAnchorPoint(Point(0.5f, 0.0f));
	_callout->setPosition(Vec2(
		stage->getPositionX(),
		origin.y + visibleSize.height / 2.0f
	));
	_callout->setScale(0.0f);

	return true;
}
