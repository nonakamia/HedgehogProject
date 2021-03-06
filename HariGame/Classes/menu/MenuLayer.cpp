#include "MenuLayer.h"
#include "Scene/BaseScene.h"
#include "Scene/TitleScene.h"
#include "Scene/StageSelectScene.h"
#include "Scene/GameScene.h"
#include "menu/setting/SettingLayer.h"

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

	//@cricket
	_buttonBank = nullptr;
	_buttonSE = nullptr;
}

MenuLayer::~MenuLayer()
{
	//@cricket
	if (_buttonBank)
	{
		_buttonBank->destroy();
		_buttonBank = nullptr;
	}
	if (_buttonSE)
	{
		_buttonSE->destroy();
		_buttonSE = nullptr;
	}
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
	_cancelButton = MenuItemImage::create(
		"menu/cancelButton.png",
		"menu/cancelButton.png",
		CC_CALLBACK_1(MenuLayer::MenuCancel, this));
	_cancelButton->setAnchorPoint(Point(0.8f, 0.8f));
	_cancelButton->setPosition(Vec2(
		menuImag->getPosition().x + menuImag->getContentSize().width / 2.0f,
		menuImag->getPosition().y + menuImag->getContentSize().height / 2.0f));
	auto cancelMenu = Menu::create(_cancelButton,nullptr);
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

	if (Director::getInstance()->getRunningScene()->getName() != "GameScene")
	{
		// 設定
		auto setting = MenuItemImage::create(
			"menu/setting.png",
			"menu/setting.png",
			CC_CALLBACK_1(MenuLayer::AddSettingLayer, this));
		setting->setPosition(Vec2(
			menuImag->getPosition().x,
			menuImag->getPosition().y * 0.8f
		));
		auto settingMenu = Menu::create(setting, nullptr);
		settingMenu->setPosition(Vec2::ZERO);
		addChild(settingMenu);
	}

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
			menuImag->getPosition().y 
		));
		auto backStageSlectMenu = Menu::create(backStageSlect, nullptr);
		backStageSlectMenu->setPosition(Vec2::ZERO);
		addChild(backStageSlectMenu);

		// ﾀｲﾄﾙ画面に戻る
		backTitle->setPosition(Vec2(
			menuImag->getPosition().x,
			menuImag->getPosition().y * 0.7f
		));

		//// 設定
		//setting->setPosition(Vec2(
		//	menuImag->getPosition().x,
		//	menuImag->getPosition().y * 0.7f
		//));
	}

	//@cricket
#ifdef CK_PLATFORM_WIN
	_buttonBank = CkBank::newBank("Resources/se/button/button.ckb");
#else
	_buttonBank = CkBank::newBank("se/button/button.ckb");
#endif
	_buttonSE = CkSound::newBankSound(_buttonBank, "decision");

	_buttonSE->play();

	return true;
}

void MenuLayer::update(float delta)
{
}

void MenuLayer::Resume()
{
	if (!_menuFlag)
	{
		_buttonSE->play();
		_cancelButton->setVisible(true);
		this->runAction(MoveTo::create(0.5f, Vec2::ZERO));
		_menuFlag = true;
	}
}

void MenuLayer::MenuCancel(Ref* pSender)
{
	if (!_menuFlag)
	{
		return;
	}

	auto scene = (BaseScene*)Director::getInstance()->getRunningScene();
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
	_buttonSE->play();

	// UserDefault呼び出し
	UserDefault* _userDef = UserDefault::getInstance();
	// ﾁｪｯｸﾎﾟｲﾝﾄの削除
	_userDef->setFloatForKey("C_POINT_X", 0.0f);
	_userDef->setFloatForKey("C_POINT_Y", 0.0f);

	// ｹﾞｰﾑｼｰﾝに画面遷移する。
	auto scene = (GameScene*)Director::getInstance()->getRunningScene();
	auto gameScene = GameScene::createGameScene(scene->GetStageName(), scene->GetMapPass());
	auto* fade = TransitionFade::create(1.0f, gameScene, Color3B::BLACK);
	Director::getInstance()->replaceScene(fade);

	_menuFlag = false;
}

void MenuLayer::BackTitleScene(Ref* pSender)
{
	if (!_menuFlag)
	{
		return;
	}
	_buttonSE->play();

	// ﾀｲﾄﾙｼｰﾝに画面遷移する。
	auto titleScene = TitleScene::createTitleScene();
	auto* fade = TransitionFadeUp::create(1.0f, titleScene);
	Director::getInstance()->replaceScene(fade);

	_menuFlag = false;
}

void MenuLayer::BackStageSelectScene(Ref* pSender)
{
	if (!_menuFlag)
	{
		return;
	}
	_buttonSE->play();

	// ｽﾃｰｼﾞｾﾚｸﾄSceneに画面遷移する。
	auto stageSelectScene = StageSelectScene::createStageSelectScene();
	auto* fade = TransitionFadeUp::create(1.0f, stageSelectScene);
	Director::getInstance()->replaceScene(fade);

	_menuFlag = false;
}

void MenuLayer::AddSettingLayer(Ref* pSender)
{
	if (!_menuFlag)
	{
		return;
	}
	_buttonSE->play();

	Size visibleSize = Director::getInstance()->getVisibleSize();
	auto settingLayer = SettingLayer::createSettingLayer();
	Director::getInstance()->getRunningScene()->addChild(settingLayer, static_cast<int>(zOlder::MENU));
	if (Director::getInstance()->getRunningScene()->getName() == "GameScene")
	{
		settingLayer->setCameraMask(static_cast<int>(CameraFlag::USER1));
	}
	_cancelButton->setVisible(false);
	this->runAction(MoveTo::create(1.0f, Vec2(-visibleSize.width, 0)));
	_menuFlag = false;
}
