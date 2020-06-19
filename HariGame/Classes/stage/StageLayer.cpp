#include "StageLayer.h"
#include "Scene/StageSelectScene.h"

USING_NS_CC;

Layer* StageLayer::createStageLayer(std::string name, std::string map)
{
	StageLayer* pRet = new(std::nothrow) StageLayer(name, map);
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

StageLayer::StageLayer(std::string name, std::string map)
{
	_name = name;
	_map = map;
	_minimumLayerPosX = 0.0f;
	_calloutFlag = false;
	_selectFlag = false;

	//@cricket
	_buttonBank = nullptr;
	_buttonSE = nullptr;
}

StageLayer::~StageLayer()
{
	//@cricket
	if (_buttonBank)
	{
		_buttonBank->destroy();
	}
	if (_buttonSE)
	{
		_buttonSE->destroy();
	}
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
	stage->setPosition(Vec2(
		origin.x + visibleSize.width / 2.0f,
		origin.y + visibleSize.height / 4.0f
	));
	auto changeListener = EventListenerTouchOneByOne::create();
	// ‰Ÿ‚³‚ê‚½Žž‚Ìˆ—
	changeListener->onTouchBegan = [this, visibleSize, stage](Touch* touch, Event* event)
	{
		// layerŽ©‘Ì‚ð‚¸‚ç‚µ‚Ä‚¢‚é‚½‚ßAthis->getPosition()‚ÅC³
		Vec2 point = touch->getLocation() - this->getPosition();
		Rect rectButton = stage->getBoundingBox();
		if (rectButton.containsPoint(point))
		{
			return true;
		}
		else
		{
			_callout->runAction(ScaleTo::create(0.1f, 0.0f));
			_stageLabel->runAction(ScaleTo::create(0.1f, 0.0f));
			_calloutFlag = false;
		}
		return false;
	};
	// —£‚³‚ê‚½Žž‚Ìˆ—
	changeListener->onTouchEnded = [this](Touch* touch, Event* event)
	{
		//@cricket
		_buttonSE->play();

		if (!_calloutFlag)
		{
			_callout->runAction(ScaleTo::create(0.1f, 1.0f));
			_stageLabel->runAction(ScaleTo::create(0.1f, 1.0f));
			_calloutFlag = true;
		}
		else
		{
			_selectFlag = true;
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

	_stageLabel =Label::createWithTTF(_name,
		"fonts/arial.ttf",
		50.0f,
		Size(_callout->getContentSize().width, _callout->getContentSize().height + 50.0f),
		TextHAlignment::CENTER,
		TextVAlignment::CENTER
	);
	addChild(_stageLabel);
	_stageLabel->setAnchorPoint(Point(0.5f, 0.0f));
	_stageLabel->setPosition(_callout->getPosition());
	_stageLabel->setColor(cocos2d::Color3B(0.0f, 0.0f, 0.0f));
	_stageLabel->setScale(0.0f);

		// add the label as a child to this layer

	//@cricket
#ifdef CK_PLATFORM_WIN
	_buttonBank = CkBank::newBank("Resources/se/button/button.ckb");
#else
	_buttonBank = CkBank::newBank("se/button/button.ckb");
#endif
	_buttonSE = CkSound::newBankSound(_buttonBank, "decision");

	return true;
}

void StageLayer::update(float delta)
{
	if ((_buttonSE) && (!_buttonSE->isPlaying()) && (_selectFlag))
	{
		auto scene = Director::getInstance()->getRunningScene();
		((StageSelectScene*)scene)->changeScene(this, _map);
		_buttonBank->destroy();
		_buttonSE->destroy();
		_buttonBank = nullptr;
		_buttonSE = nullptr;
	}
}

void StageLayer::SetMinimumLayerPosX(float posX)
{
	_minimumLayerPosX = posX;
}

float StageLayer::GetMinimumLayerPosX()
{
	return _minimumLayerPosX;
}
