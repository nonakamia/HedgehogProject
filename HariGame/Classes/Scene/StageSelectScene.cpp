#include "StageSelectScene.h"
#include "GameScene.h"

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
	BaseScene();
}

StageSelectScene::~StageSelectScene()
{
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

	auto stage = Sprite::create("StageSelect/stage.png");
	addChild(stage, static_cast<int>(zOlder::OBSTACLES));
	stage->setPosition(Vec2(origin.x + visibleSize.width / 2.0f,
		origin.y + visibleSize.height/4.0f));

	stage->getBoundingBox();

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
			return false;
		}
	};
	// —£‚³‚ê‚½Žž‚Ìˆ—
	changeListener->onTouchEnded = [this](Touch* touch, Event* event)
	{
		changeScene(this);
		return true;
	};
	getEventDispatcher()->addEventListenerWithSceneGraphPriority(changeListener, this);

	return true;
}

void StageSelectScene::changeScene(Ref* pSender)
{
	if (_changeSceneFlag)
	{
		return;
	}

	if (!_changeSceneFlag)
	{
		// ƒZƒŒƒNƒgƒV[ƒ“‚É‰æ–Ê‘JˆÚ‚·‚éB
		auto GameScene = GameScene::createGameScene();
		auto* fade = TransitionFade::create(1.0f, GameScene,Color3B::BLACK);
		// TitleScene‚ð”jŠü‚µ‚ÄGameScene‚É‘JˆÚ‚·‚é
		Director::getInstance()->replaceScene(fade);

		_changeSceneFlag = true;
	}
}
