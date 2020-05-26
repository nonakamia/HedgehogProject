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

	auto image = Sprite::create("image2.png");
	addChild(image, static_cast<int>(zOlder::BG));
	image->setPosition(Vec2(origin.x + visibleSize.width / 2.0f,
		origin.y + visibleSize.height / 2.0f));

	auto stage = Sprite::create("StageSelect/stage.png");
	addChild(stage, static_cast<int>(zOlder::OBSTACLES));
	stage->setPosition(Vec2(origin.x + visibleSize.width / 2.0f,
		origin.y + visibleSize.height/4.0f));

	stage->getBoundingBox();

	auto changeListener = EventListenerTouchOneByOne::create();
	// 押された時の処理
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
	// 離された時の処理
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
		// セレクトシーンに画面遷移する。
		auto gameScene = GameScene::createGameScene();
		auto* fade = TransitionFade::create(1.0f, gameScene,Color3B::BLACK);
		// TitleSceneを破棄してGameSceneに遷移する
		Director::getInstance()->replaceScene(fade);

		_changeSceneFlag = true;
	}
}
