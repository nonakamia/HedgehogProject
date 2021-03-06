#include "StageSelectScene.h"
#include "Split/Split.h"
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

	touchPos = Vec2::ZERO;

	//@cricket
	_selectSound = nullptr;
}

StageSelectScene::~StageSelectScene()
{
	// シーン切替時に×ボタン押したら吹っ飛ぶのを防ぐ
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

	// 外部ﾃﾞｰﾀ読み込み
	std::string defaultIfs = FileUtils::getInstance()->getStringFromFile("csv/stage.csv");
	if (defaultIfs == "")
	{
		return false;
	}
	ValueVector defaultCsvSplit = Split::split(defaultIfs, "\n");
	for (int i = 1; i < (int)defaultCsvSplit.size(); i++)
	{
		ValueVector defaultCsvData = Split::split(defaultCsvSplit.at(i).asString(), ",");
		StageData stageData = StageData();
		stageData.stageName = defaultCsvData.at(0).asString();
		stageData.imagePass = defaultCsvData.at(1).asString();
		stageData.mapData = defaultCsvData.at(2).asString();
		_stage.emplace_back(stageData);
	}

	// ﾁｪｯｸﾎﾟｲﾝﾄの初期化
	UserDefault* _userDef = UserDefault::getInstance();
	_userDef->setFloatForKey("C_POINT_X", 0.0f);
	_userDef->setFloatForKey("C_POINT_Y", 0.0f);

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	// 背景
	auto image = Sprite::create("image2.png");
	addChild(image, static_cast<int>(zOlder::BG));
	image->setPosition(Vec2(origin.x + visibleSize.width / 2.0f,
		origin.y + visibleSize.height / 2.0f));

	// ｽﾃｰｼﾞ
	// stage.csvに記載
	float posX = 0;
	for (auto data : _stage)
	{
		auto stage = StageLayer::createStageLayer(data);
		stage->setPosition(posX, 0.0f);
		stage->setName("stage");
		addChild(stage, static_cast<int>(zOlder::OBSTACLES));
		((StageLayer*)stage)->SetMinimumLayerPosX(posX);
		stage->scheduleUpdate();
		posX += 200.0f;
	}

	// ﾒﾆｭｰﾎﾞﾀﾝ
	_button = MenuItemImage::create(
		"menu/menuButton.png",
		"menu/menuButton.png",
		CC_CALLBACK_1(StageSelectScene::SetMenu, this));
	_button->setPosition(Vec2(origin.x + 50.0f,
		origin.y + visibleSize.height - 50.0f));
	auto menu = Menu::create(_button,nullptr);
	menu->setPosition(Vec2::ZERO);
	addChild(menu,static_cast<int>(zOlder::BUTTON));

	// ｽﾃｰｼﾞｽｸﾛｰﾙ
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

bool StageSelectScene::changeScene(Ref* pSender, std::string stageName, std::string map)
{
	if ((_changeSceneFlag)||(_menuFlag)||(map == ""))
	{
		return false;
	}

	if (!_changeSceneFlag)
	{
		// ｹﾞｰﾑｼｰﾝに画面遷移する。
		auto gameScene = GameScene::createGameScene(stageName, map);
		auto* fade = TransitionFade::create(1.0f, gameScene,Color3B::BLACK);
		// TitleSceneを破棄してGameSceneに遷移する
		Director::getInstance()->replaceScene(fade);

		//@cricket
		_selectSound->destroy();
		_selectSound = nullptr;

		_changeSceneFlag = true;

		return true;
	}

	return false;
}

void StageSelectScene::AddScrollAction()
{
	// 全てのｽﾃｰｼﾞが画面内にあるか
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
				float x = (stage->getPosition().x < 0.0f ? 1.0f : -1.0f);
				stage->setPosition(stage->getPosition().x + x, stage->getPosition().y);
			}
		}
		return false;
	};

	auto listener = EventListenerTouchOneByOne::create();
	// 押した時
	listener->onTouchBegan = [&](Touch* touch, Event* event)->bool
	{
		if (_menuFlag)
		{
			return false;
		}

		touchPos = touch->getLocation();
		return true;
	};

	// 押してから動かした時
	listener->onTouchMoved = [&](Touch* touch, Event* event)->bool
	{
		if (_menuFlag)
		{
			return false;
		}

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
							stage->setPosition(stage->getPosition().x + 10.0f, stage->getPosition().y);
						}
						else if (distance < -10.0f)
						{
							stage->setPosition(stage->getPosition().x - 10.0f, stage->getPosition().y);
						}
					}
				}
				touchPos = touch->getLocation();
			}
		}
		return true;
	};

	// 離した時
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
