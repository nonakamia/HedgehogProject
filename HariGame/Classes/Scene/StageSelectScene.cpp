#include "StageSelectScene.h"
#include "Split/Split.h"
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

	touchPos = Vec2::ZERO;

	//@cricket
	_selectSound = nullptr;
}

StageSelectScene::~StageSelectScene()
{
	// ƒV[ƒ“Ø‘ÖŽž‚É~ƒ{ƒ^ƒ“‰Ÿ‚µ‚½‚ç‚Á”ò‚Ô‚Ì‚ð–h‚®
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

	// ŠO•”ÃÞ°À“Ç‚Ýž‚Ý
	std::string defaultIfs = FileUtils::getInstance()->getStringFromFile("csv/stage.csv");
	if (defaultIfs == "")
	{
		return false;
	}
	ValueVector defaultCsvSplit = Split::split(defaultIfs, "\n");
	for (int i = 1; i < (int)defaultCsvSplit.size(); i++)
	{
		ValueVector defaultCsvData = Split::split(defaultCsvSplit.at(i).asString(), ",");
		_stage.emplace_back(defaultCsvData.at(0).asString(), defaultCsvData.at(1).asString());
	}

	// Áª¯¸Îß²ÝÄ‚Ì‰Šú‰»
	UserDefault* _userDef = UserDefault::getInstance();
	_userDef->setFloatForKey("C_POINT_X", 0.0f);
	_userDef->setFloatForKey("C_POINT_Y", 0.0f);

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	// ”wŒi
	auto image = Sprite::create("image2.png");
	addChild(image, static_cast<int>(zOlder::BG));
	image->setPosition(Vec2(origin.x + visibleSize.width / 2.0f,
		origin.y + visibleSize.height / 2.0f));

	// ½Ã°¼Þ
	// stage.csv‚É‹LÚ
	float posX = 0;
	for (auto data : _stage)
	{
		auto stage = StageLayer::createStageLayer(data.first, data.second);
		stage->setPositionX(posX);
		stage->setName("stage");
		addChild(stage, static_cast<int>(zOlder::OBSTACLES));
		((StageLayer*)stage)->SetMinimumLayerPosX(posX);
		stage->scheduleUpdate();
		posX += 200.0f;
	}

	// ÒÆ­°ÎÞÀÝ
	_button = MenuItemImage::create(
		"menu/menuButton.png",
		"menu/menuButton.png",
		CC_CALLBACK_1(StageSelectScene::SetMenu, this));
	_button->setPosition(Vec2(origin.x + 50.0f,
		origin.y + visibleSize.height - 50.0f));
	auto menu = Menu::create(_button,nullptr);
	menu->setPosition(Vec2::ZERO);
	addChild(menu,static_cast<int>(zOlder::BUTTON));

	// ½Ã°¼Þ½¸Û°Ù
	AddScrollAction();

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
	_button->setVisible(true);
}

void StageSelectScene::changeScene(Ref* pSender, std::string map)
{
	if ((_changeSceneFlag)||(_menuFlag)||(map == ""))
	{
		return;
	}

	if (!_changeSceneFlag)
	{
		// ¹Þ°Ñ¼°Ý‚É‰æ–Ê‘JˆÚ‚·‚éB
		auto gameScene = GameScene::createGameScene(map);
		auto* fade = TransitionFade::create(1.0f, gameScene,Color3B::BLACK);
		// TitleScene‚ð”jŠü‚µ‚ÄGameScene‚É‘JˆÚ‚·‚é
		Director::getInstance()->replaceScene(fade);

		//@cricket
		_selectSound->destroy();
		_selectSound = nullptr;

		_changeSceneFlag = true;
	}
}

void StageSelectScene::AddScrollAction()
{
	// ‘S‚Ä‚Ì½Ã°¼Þ‚ª‰æ–Ê“à‚É‚ ‚é‚©
	auto checkStage = [](Vector<Node*> children)->bool
	{
		Size visibleSize = Director::getInstance()->getVisibleSize();
		for (auto stage : children)
		{
			if (stage->getName() == "stage")
			{
				if ((stage->getPosition().x >= 0.0f) && (stage->getPosition().x  <= ((StageLayer*)stage)->GetMinimumLayerPosX()))
				{
					return true;
				}
			}
		}

		for (auto stage : children)
		{
			if (stage->getName() == "stage")
			{
				if (stage->getPosition().x < 0.0f)
				{
					stage->setPositionX(stage->getPosition().x + 1.0f);
				}
				else
				{
					stage->setPositionX(stage->getPosition().x - 1.0f);
				}
			}
		}
		return false;
	};

	auto listener = EventListenerTouchOneByOne::create();
	// ‰Ÿ‚µ‚½Žž
	listener->onTouchBegan = [&](Touch* touch, Event* event)->bool
	{
		touchPos = touch->getLocation();
		return true;
	};

	// ‰Ÿ‚µ‚Ä‚©‚ç“®‚©‚µ‚½Žž
	listener->onTouchMoved = [&](Touch* touch, Event* event)->bool
	{
		if (checkStage(this->getChildren()))
		{
			auto distance = touch->getLocation().x - touchPos.x;
			if (abs(distance) > 10.0f)
			{
				for (auto stage : this->getChildren())
				{
					if (stage->getName() == "stage")
					{
						if (distance > 10.0f)
						{
							stage->setPositionX(stage->getPosition().x + 10.0f);
						}
						else if (distance < -10.0f)
						{
							stage->setPositionX(stage->getPosition().x - 10.0f);
						}
					}
				}
				touchPos = touch->getLocation();
			}
		}
		return true;
	};

	// —£‚µ‚½Žž
	listener->onTouchEnded = [&](Touch* touch, Event* event)->bool
	{
		touchPos = Vec2::ZERO;
		return true;
	};
	this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
}

void StageSelectScene::SetMenu(Ref* pSender)
{
	if (!_menuFlag)
	{
		auto menuLayer = MenuLayer::createMenuLayer();
		this->addChild(menuLayer, static_cast<int>(zOlder::MENU));
		_button->setVisible(false);
		_menuFlag = true;
	}
}
