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
	_endGameFlag = false;
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

	// 押してから動かした時
	listener->onTouchMoved = [this](Touch* touch, Event* event)->bool
	{
		return true;
	};

	// 離した時
	listener->onTouchEnded = [this](Touch* touch, Event* event)->bool
	{
		if ((abs(_touchPoint.x- touch->getLocation().x)<50.0f)&&(!_endGameFlag))
		{
			changeScene(this);
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
				Director::getInstance()->end();
			}
		}
		return true;
	};
	this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(keyListener, this);

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
		// セレクトシーンに画面遷移する。
		auto stageSelectScene = StageSelectScene::createStageSelectScene();
		auto* fade = TransitionFadeUp::create(1.0f, stageSelectScene);
		// TitleSceneを破棄してStageSelectSceneに遷移する
		Director::getInstance()->replaceScene(fade);

		_changeSceneFlag = true;
	}
}

void TitleScene::SetEndGame(Ref* pSender)
{
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
