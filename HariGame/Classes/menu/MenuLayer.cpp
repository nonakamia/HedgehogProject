#include "MenuLayer.h"
#include "Scene/BaseScene.h"
#include "Scene/TitleScene.h"
#include "Scene/StageSelectScene.h"
#include "Scene/GameScene.h"

USING_NS_CC;

Layer* MenuLayer::createMenuLayer()
{
	MenuLayer* pRet = new(std::nothrow) MenuLayer();
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

MenuLayer::MenuLayer()
{
	_menuFlag = true;
}

MenuLayer::~MenuLayer()
{
}

bool MenuLayer::init()
{
	if (!Layer::init())
	{
		return false;
	}
	setName("MenuLayer");

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto menuImag = Sprite::create("menu/menuImage.png");
	addChild(menuImag);
	menuImag->setPosition(Vec2(
		origin.x + visibleSize.width / 2.0f,
		origin.y + visibleSize.height / 2.0f
	));


	// ｷｬﾝｾﾙﾎﾞﾀﾝ
	auto cancelButton = MenuItemImage::create(
		"menu/cancelButton.png",
		"menu/cancelButton.png",
		CC_CALLBACK_1(MenuLayer::MenuCancel, this));
	cancelButton->setAnchorPoint(Point(0.0f, 0.0f));
	cancelButton->setPosition(Vec2(
		menuImag->getPosition().x + menuImag->getContentSize().width / 2.0f,
		menuImag->getPosition().y + menuImag->getContentSize().height / 2.0f));
	auto cancelMenu = Menu::create(cancelButton,nullptr);
	cancelMenu->setPosition(Vec2::ZERO);
	addChild(cancelMenu);

	// ﾀｲﾄﾙに戻る
	auto backTitle = MenuItemImage::create(
		"menu/backTitle.png",
		"menu/backTitle.png",
		CC_CALLBACK_1(MenuLayer::BackTitleScene, this));
	backTitle->setPosition(Vec2(
		menuImag->getPosition().x,
		menuImag->getPosition().y * 1.2f
	));
	auto backTitleMenu = Menu::create(backTitle, nullptr);
	backTitleMenu->setPosition(Vec2::ZERO);
	addChild(backTitleMenu);

	// 設定
	auto setting = Sprite::create("menu/setting.png");
	addChild(setting);
	setting->setPosition(Vec2(
		menuImag->getPosition().x,
		menuImag->getPosition().y * 0.8f
	));

	// GameSceneのみでの表示
	if (Director::getInstance()->getRunningScene()->getName() == "GameScene")
	{
		// はじめから
		auto fromScratch = MenuItemImage::create(
			"menu/fromScratch.png",
			"menu/fromScratch.png",
			CC_CALLBACK_1(MenuLayer::SromScratch, this));
		fromScratch->setPosition(Vec2(
			menuImag->getPosition().x,
			menuImag->getPosition().y * 1.3f
		));
		auto fromScratchMenu = Menu::create(fromScratch, nullptr);
		fromScratchMenu->setPosition(Vec2::ZERO);
		addChild(fromScratchMenu);

		// ｽﾃｰｼﾞｾﾚｸﾄに戻る
		auto backStageSlect = MenuItemImage::create(
			"menu/backStageSelect.png",
			"menu/backStageSelect.png",
			CC_CALLBACK_1(MenuLayer::BackStageSelectScene, this));
		backStageSlect->setPosition(Vec2(
			menuImag->getPosition().x,
			menuImag->getPosition().y * 1.1f
		));
		auto backStageSlectMenu = Menu::create(backStageSlect, nullptr);
		backStageSlectMenu->setPosition(Vec2::ZERO);
		addChild(backStageSlectMenu);

		// ﾀｲﾄﾙ画面に戻る
		backTitle->setPosition(Vec2(
			menuImag->getPosition().x,
			menuImag->getPosition().y * 0.9f
		));

		// 設定
		setting->setPosition(Vec2(
			menuImag->getPosition().x,
			menuImag->getPosition().y * 0.7f
		));
	}

	return true;
}

void MenuLayer::MenuCancel(Ref* pSender)
{
	if (!_menuFlag)
	{
		return;
	}

	auto scene = (BaseScene*)Director::getInstance()->getRunningScene();
	//scene->SetMenuFlag(false);
	scene->Resume();
	
	this->removeFromParentAndCleanup(true);

	_menuFlag = false;
}

void MenuLayer::SromScratch(Ref* pSender)
{
	if (!_menuFlag)
	{
		return;
	}

	// ｹﾞｰﾑｼｰﾝに画面遷移する。
	auto gameScene = GameScene::createGameScene();
	auto* fade = TransitionFade::create(1.0f, gameScene, Color3B::BLACK);
	// TitleSceneを破棄してGameSceneに遷移する
	Director::getInstance()->replaceScene(fade);

	_menuFlag = false;
}

void MenuLayer::BackTitleScene(Ref* pSender)
{
	if (!_menuFlag)
	{
		return;
	}

	// ﾀｲﾄﾙｼｰﾝに画面遷移する。
	auto titleScene = TitleScene::createTitleScene();
	// 0.5秒かけてホワイトアウトしてタイトルに移動する
	auto* fade = TransitionFadeUp::create(1.0f, titleScene);
	// PauseLayerを破棄してtitleSceneに遷移する
	Director::getInstance()->replaceScene(fade);

	_menuFlag = false;
}

void MenuLayer::BackStageSelectScene(Ref* pSender)
{
	if (!_menuFlag)
	{
		return;
	}

	// ｽﾃｰｼﾞｾﾚｸﾄSceneに画面遷移する。
	auto stageSelectScene = StageSelectScene::createStageSelectScene();
	// 0.5秒かけてホワイトアウトしてタイトルに移動する
	auto* fade = TransitionFadeUp::create(1.0f, stageSelectScene);
	// PauseLayerを破棄してtitleSceneに遷移する
	Director::getInstance()->replaceScene(fade);

	_menuFlag = false;
}
