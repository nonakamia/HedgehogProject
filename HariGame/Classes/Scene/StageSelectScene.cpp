#include "StageSelectScene.h"
#include "GameScene.h"
#include "menu/MenuLayer.h"

USING_NS_CC;

cocos2d::Scene* StageSelectScene::createStageSelectScene()
{
	return StageSelectScene::create();
}

static void problemLoading(const char* filename)
{
	printf("Error while loading: %s\n", filename);
	printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in StageSelectScene.cpp\n");
}

StageSelectScene::StageSelectScene()
{
	_changeSceneFlag = false;
	_menuFlag = false;

	_calloutFlag = false;
}

StageSelectScene::~StageSelectScene()
{
	// ƒV[ƒ“Ø‘ÖŽž‚É~ƒ{ƒ^ƒ“‰Ÿ‚µ‚½‚ç‚Á”ò‚Ô‚Ì‚ð–h‚®
	if (_running)
	{
		onExit();
	}
}

bool StageSelectScene::init()
{
	if (!Scene::init())
	{
		return false;
	}
	setName("StageSelectScene");

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto label = Label::createWithTTF("StageSelectScene", "fonts/Marker Felt.ttf", 24);
	if (label == nullptr)
	{
		problemLoading("'fonts/Marker Felt.ttf'");
	}
	else
	{
		// position the label on the center of the screen
		label->setPosition(Vec2(origin.x + visibleSize.width / 2.0f,
			origin.y + visibleSize.height - label->getContentSize().height));
		// add the label as a child to this layer
		this->addChild(label, 1);
	}

	auto image = Sprite::create("image2.png");
	addChild(image, static_cast<int>(zOlder::BG));
	image->setPosition(Vec2(origin.x + visibleSize.width / 2.0f,
		origin.y + visibleSize.height / 2.0f));

	auto stage = Sprite::create("StageSelect/stage.png");
	addChild(stage, static_cast<int>(zOlder::OBSTACLES));
	stage->setPosition(Vec2(origin.x + visibleSize.width / 2.0f,
		origin.y + visibleSize.height/4.0f));

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
			changeScene(this);
		}
		return true;
	};
	getEventDispatcher()->addEventListenerWithSceneGraphPriority(changeListener, this);

	_callout = Sprite::create("StageSelect/callout.png");
	addChild(_callout);
	_callout->setAnchorPoint(Point(0.5f, 0.0f));
	_callout->setPosition(Vec2(
		origin.x + visibleSize.width / 2.0f,
		origin.y + visibleSize.height / 2.0f
	));
	_callout->setScale(0.0f);

	// ÒÆ­°ÎÞÀÝ
	auto button= MenuItemImage::create(
		"menu/menuButton.png",
		"menu/menuButton.png",
		CC_CALLBACK_1(StageSelectScene::SetMenu, this));
	button->setPosition(Vec2(origin.x + 50.0f,
		origin.y + visibleSize.height - 50.0f));
	auto menu = Menu::create(button,nullptr);
	menu->setPosition(Vec2::ZERO);
	addChild(menu,static_cast<int>(zOlder::BUTTON));

	return true;
}

void StageSelectScene::Resume()
{
	_menuFlag = false;
}

void StageSelectScene::changeScene(Ref* pSender)
{
	if ((_changeSceneFlag)||(_menuFlag))
	{
		return;
	}

	if (!_changeSceneFlag)
	{
		// ¹Þ°Ñ¼°Ý‚É‰æ–Ê‘JˆÚ‚·‚éB
		auto gameScene = GameScene::createGameScene();
		auto* fade = TransitionFade::create(1.0f, gameScene,Color3B::BLACK);
		// TitleScene‚ð”jŠü‚µ‚ÄGameScene‚É‘JˆÚ‚·‚é
		Director::getInstance()->replaceScene(fade);

		_changeSceneFlag = true;
	}
}

void StageSelectScene::SetMenu(Ref* pSender)
{
	if (!_menuFlag)
	{
		auto menuLayer = MenuLayer::createMenuLayer();
		this->addChild(menuLayer, static_cast<int>(zOlder::MENU));
		_menuFlag = true;
	}
}
