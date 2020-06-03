#include "TitleScene.h"
#include "StageSelectScene.h"

USING_NS_CC;

cocos2d::Scene* TitleScene::createTitleScene()
{
	return TitleScene::create();
}

TitleScene::TitleScene()
{
	_changeSceneFlag = false;
	_endGameFlag = false;

	//@cricket
	_titleSound = nullptr;
	_buttonBank = nullptr;
	_buttonSE = nullptr;
}

TitleScene::~TitleScene()
{
	// シーン切替時に×ボタン押したら吹っ飛ぶのを防ぐ
	if (_running)
	{
		onExit();
	}

	if (_titleSound)
	{
		_titleSound->destroy();
	}
	if (_buttonBank)
	{
		_buttonBank->destroy();
	}
	if (_buttonSE)
	{
		_buttonSE->destroy();
	}
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

	auto titleImage = Sprite::create("image.png");
	addChild(titleImage, static_cast<int>(zOlder::BG));
	titleImage->setPosition(Vec2(
		origin.x + visibleSize.width / 2.0f,
		origin.y + visibleSize.height / 2.0f));

	auto gameName = Sprite::create("TitleScene/gameName.png");
	addChild(gameName);
	gameName->setPosition(Vec2(
		origin.x + visibleSize.width / 2.0f,
		origin.y + visibleSize.height / 1.5f
	));

	// tap to start
	auto tapImage = Sprite::create("TitleScene/tapToStart.png");
	addChild(tapImage);
	tapImage->setPosition(Vec2(
		origin.x + visibleSize.width / 2.0f,
		origin.y + visibleSize.height / 4.0f
	));
	tapImage->runAction(
		RepeatForever::create(
			Sequence::create(
				FadeOut::create(1.0f),
				DelayTime::create(0.5f),
				FadeIn::create(1.0f),
				nullptr
		)));

	// ｹﾞｰﾑ終了
	_endGameImage = Sprite::create("endGame/endGameImage.png");
	addChild(_endGameImage);
	_endGameImage->setPosition(Vec2(origin.x + visibleSize.width / 2.0f,
		origin.y + visibleSize.height / 2.0f));
	_endGameImage->setScale(0.0f);

	_yesButton = MenuItemImage::create(
		"endGame/yes.png",
		"endGame/yes.png",
		CC_CALLBACK_1(TitleScene::EndGame,this));
	_yesButton->setPosition(Vec2(origin.x + visibleSize.width / 3.0f,
		origin.y + visibleSize.height / 2.5f));
	_yesButton->setVisible(false);
	auto yesMenu = Menu::create(_yesButton, nullptr);
	yesMenu->setPosition(Vec2::ZERO);
	this->addChild(yesMenu);

	_noButton = MenuItemImage::create(
		"endGame/no.png",
		"endGame/no.png",
		CC_CALLBACK_1(TitleScene::SetEndGame,this)
	);
	_noButton->setPosition(Vec2(origin.x + visibleSize.width / 1.5f,
		origin.y + visibleSize.height / 2.5f));
	_noButton->setVisible(false);
	auto noMenu = Menu::create(_noButton, nullptr);
	noMenu->setPosition(Vec2::ZERO);
	this->addChild(noMenu);

	// ﾀｯﾁｲﾍﾞﾝﾄ
	auto listener = EventListenerTouchOneByOne::create();
	// 押した時
	listener->onTouchBegan = [this](Touch* touch, Event* event)->bool
	{
		_touchPoint = touch->getLocation();
	
		return true;
	};
	// 離した時
	listener->onTouchEnded = [this](Touch* touch, Event* event)->bool
	{
		if ((abs(_touchPoint.x- touch->getLocation().x)<50.0f)&&(!_endGameFlag))
		{
			SetChangeScene(this);
		}
		if (_endGameFlag)
		{
			SetEndGame(this);
		}
		return true;
	};
	this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);

	auto keyListener = EventListenerKeyboard::create();
	keyListener->onKeyReleased = [this](cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event)->bool
	{
		// ESCで終了(androidでは戻るﾎﾞﾀﾝと連動)
		if (keyCode == cocos2d::EventKeyboard::KeyCode::KEY_ESCAPE)
		{
			if (!_endGameFlag)
			{
				SetEndGame(this);
			}
			else
			{
#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
				Director::getInstance()->end();
#endif
			}
		}
		return true;
	};
	this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(keyListener, this);

	//@cricket
#ifdef CK_PLATFORM_WIN
	_titleSound = CkSound::newStreamSound("Resources/sound/SeaofTrees.cks");
	_buttonBank = CkBank::newBank("Resources/se/button/button.ckb");
#else
	_titleSound = CkSound::newStreamSound("sound/SeaofTrees.cks");
	_buttonBank = CkBank::newBank("se/button/button.ckb");
#endif
	_buttonSE = CkSound::newBankSound(_buttonBank, "decision");
	_titleSound->setLoopCount(-1);
	_titleSound->play();

	this->scheduleUpdate();
	return true;
}

void TitleScene::update(float delta)
{	
	if ((_buttonSE)&&(!_buttonSE->isPlaying()))
	{
		ChangeScene();
	}

	//@cricket
	CkUpdate();
}

void TitleScene::SetChangeScene(Ref* pSender)
{
	if (_changeSceneFlag)
	{
		return;
	}

	_buttonSE->play();

	_changeSceneFlag = true;
}

void TitleScene::ChangeScene()
{
	if (!_changeSceneFlag)
	{
		return;
	}

	// セレクトシーンに画面遷移する。
	auto stageSelectScene = StageSelectScene::createStageSelectScene();
	auto* fade = TransitionFadeUp::create(1.0f, stageSelectScene);
	// TitleSceneを破棄してStageSelectSceneに遷移する
	Director::getInstance()->replaceScene(fade);

	//@cricket
	_titleSound->destroy();
	_buttonBank->destroy();
	_buttonSE->destroy();
	_titleSound = nullptr;
	_buttonBank = nullptr;
	_buttonSE = nullptr;	
}

void TitleScene::SetEndGame(Ref* pSender)
{
	_buttonSE->play();
	if (!_endGameFlag)
	{
		_endGameImage->runAction(ScaleTo::create(0.2f, 1.0f));
		_yesButton->setVisible(true);
		_noButton->setVisible(true);
	}
	else if (_endGameFlag)
	{
		_endGameImage->runAction(ScaleTo::create(0.2f, 0.0f));
		_yesButton->setVisible(false);
		_noButton->setVisible(false);
	}
	
	_endGameFlag = !_endGameFlag;
}

void TitleScene::EndGame(Ref* pSender)
{
	Director::getInstance()->end();
}

void TitleScene::Resume()
{
}
