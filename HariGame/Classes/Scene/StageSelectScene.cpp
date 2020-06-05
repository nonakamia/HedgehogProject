#include "StageSelectScene.h"
#include "GameScene.h"
#include "stage/StageLayer.h"
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

	//@cricket
	_selectSound = nullptr;
}

StageSelectScene::~StageSelectScene()
{
	// ƒV[ƒ“Ø‘Ö‚É~ƒ{ƒ^ƒ“‰Ÿ‚µ‚½‚ç‚Á”ò‚Ô‚Ì‚ğ–h‚®
	if (_running)
	{
		onExit();
	}

	//@cricket
	if (_selectSound)
	{
		_selectSound->destroy();
		_selectSound = nullptr;
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

	auto image = Sprite::create("image2.png");
	addChild(image, static_cast<int>(zOlder::BG));
	image->setPosition(Vec2(origin.x + visibleSize.width / 2.0f,
		origin.y + visibleSize.height / 2.0f));

	// ½Ã°¼Ş
	// •¡”ì¬—\’è‚¾‚©‚çƒNƒ‰ƒX‰»‚·‚é—\’è
	auto stage = StageLayer::createStageLayer("stage/stage_0.tmx",
		Vec2(origin.x + visibleSize.width / 2.0f,
		origin.y + visibleSize.height / 4.0f));
	addChild(stage, static_cast<int>(zOlder::OBSTACLES));
	stage->scheduleUpdate();

	auto stage2 = StageLayer::createStageLayer("stage/stage_1.tmx",
		Vec2(origin.x + visibleSize.width / 1.5f,
			origin.y + visibleSize.height / 4.0f));
	addChild(stage2, static_cast<int>(zOlder::OBSTACLES));
	stage2->scheduleUpdate();

	// ÒÆ­°ÎŞÀİ
	auto button= MenuItemImage::create(
		"menu/menuButton.png",
		"menu/menuButton.png",
		CC_CALLBACK_1(StageSelectScene::SetMenu, this));
	button->setPosition(Vec2(origin.x + 50.0f,
		origin.y + visibleSize.height - 50.0f));
	auto menu = Menu::create(button,nullptr);
	menu->setPosition(Vec2::ZERO);
	addChild(menu,static_cast<int>(zOlder::BUTTON));

	//@cricket
#ifdef CK_PLATFORM_WIN
	_selectSound = CkSound::newStreamSound("Resources/sound/stageSelect.cks");
#else
	_selectSound = CkSound::newStreamSound("sound/stageSelect.cks");
#endif
	_selectSound->setLoopCount(-1);
	_selectSound->play();

	this->scheduleUpdate();
	return true;
}

void StageSelectScene::update(float delta)
{
	//@cricket
	CkUpdate();
}

void StageSelectScene::Resume()
{
	_menuFlag = false;
}

void StageSelectScene::changeScene(Ref* pSender, std::string map)
{
	if ((_changeSceneFlag)||(_menuFlag))
	{
		return;
	}

	if (!_changeSceneFlag)
	{
		// ¹Ş°Ñ¼°İ‚É‰æ–Ê‘JˆÚ‚·‚éB
		auto gameScene = GameScene::createGameScene(map);
		auto* fade = TransitionFade::create(1.0f, gameScene,Color3B::BLACK);
		// TitleScene‚ğ”jŠü‚µ‚ÄGameScene‚É‘JˆÚ‚·‚é
		Director::getInstance()->replaceScene(fade);

		//@cricket
		_selectSound->destroy();
		_selectSound = nullptr;

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
