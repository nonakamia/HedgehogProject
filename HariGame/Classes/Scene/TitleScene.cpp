#include "TitleScene.h"
#include "StageSelectScene.h"

USING_NS_CC;

cocos2d::Scene* TitleScene::createTitleScene()
{
	return TitleScene::create();
}

TitleScene::TitleScene()
{
	BaseScene();
}

TitleScene::~TitleScene()
{
}

static void problemLoading(const char* filename)
{
	printf("Error while loading: %s\n", filename);
	printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in TitleScene.cpp\n");
}

bool TitleScene::init()
{

	if (!Scene::init())
	{
		return false;
	}
	setName("TitleScene");

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto label = Label::createWithTTF("TitleScene", "fonts/Marker Felt.ttf", 24);
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
		this->addChild(label, 2);
	}

	auto titleImage = Sprite::create("image.png");
	addChild(titleImage, static_cast<int>(zOlder::BG));
	titleImage->setPosition(Vec2(origin.x + visibleSize.width / 2.0f,
		origin.y + visibleSize.height / 2.0f));

	auto listener = EventListenerTouchOneByOne::create();

	// ‰Ÿ‚µ‚½Žž
	listener->onTouchBegan = [this](Touch* touch, Event* event)->bool
	{
		Size visibleSize = Director::getInstance()->getVisibleSize();
	
		return true;
	};

	// ‰Ÿ‚µ‚Ä‚©‚ç“®‚©‚µ‚½Žž
	listener->onTouchMoved = [this](Touch* touch, Event* event)->bool
	{
		return true;
	};

	// —£‚µ‚½Žž
	listener->onTouchEnded = [this](Touch* touch, Event* event)->bool
	{
		changeScene(this);
		return true;
	};
	this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);

	return true;
}

void TitleScene::changeScene(Ref* pSender)
{
	if (_changeSceneFlag)
	{
		return;
	}

	if (!_changeSceneFlag)
	{
		// ƒZƒŒƒNƒgƒV[ƒ“‚É‰æ–Ê‘JˆÚ‚·‚éB
		auto stageSelectScene = StageSelectScene::createStageSelectScene();
		auto* fade = TransitionFadeUp::create(1.0f, stageSelectScene);
		// TitleScene‚ð”jŠü‚µ‚ÄStageSelectScene‚É‘JˆÚ‚·‚é
		Director::getInstance()->replaceScene(fade);

		_changeSceneFlag = true;
	}
}
