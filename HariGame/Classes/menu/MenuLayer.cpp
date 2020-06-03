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
	}
	if (_buttonSE)
	{
		_buttonSE->destroy();
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


	// ��ݾ�����
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

	// ���قɖ߂�
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

	// �ݒ�
	auto setting = Sprite::create("menu/setting.png");
	addChild(setting);
	setting->setPosition(Vec2(
		menuImag->getPosition().x,
		menuImag->getPosition().y * 0.8f
	));

	// GameScene�݂̂ł̕\��
	if (Director::getInstance()->getRunningScene()->getName() == "GameScene")
	{
		// �͂��߂���
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

		// �ð�޾ڸĂɖ߂�
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

		// ���ى�ʂɖ߂�
		backTitle->setPosition(Vec2(
			menuImag->getPosition().x,
			menuImag->getPosition().y * 0.9f
		));

		// �ݒ�
		setting->setPosition(Vec2(
			menuImag->getPosition().x,
			menuImag->getPosition().y * 0.7f
		));
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

void MenuLayer::MenuCancel(Ref* pSender)
{
	if (!_menuFlag)
	{
		return;
	}

	//@cricket
	_buttonBank->destroy();
	_buttonSE->destroy();
	_buttonBank = nullptr;
	_buttonSE = nullptr;

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

	// �ްѼ�݂ɉ�ʑJ�ڂ���B
	auto gameScene = GameScene::createGameScene(((GameScene*)Director::getInstance()->getRunningScene())->GetMapName());
	auto* fade = TransitionFade::create(1.0f, gameScene, Color3B::BLACK);
	// TitleScene��j������GameScene�ɑJ�ڂ���
	Director::getInstance()->replaceScene(fade);

	_menuFlag = false;
}

void MenuLayer::BackTitleScene(Ref* pSender)
{
	if (!_menuFlag)
	{
		return;
	}

	// ���ټ�݂ɉ�ʑJ�ڂ���B
	auto titleScene = TitleScene::createTitleScene();
	// 0.5�b�����ăz���C�g�A�E�g���ă^�C�g���Ɉړ�����
	auto* fade = TransitionFadeUp::create(1.0f, titleScene);
	// PauseLayer��j������titleScene�ɑJ�ڂ���
	Director::getInstance()->replaceScene(fade);

	_menuFlag = false;
}

void MenuLayer::BackStageSelectScene(Ref* pSender)
{
	if (!_menuFlag)
	{
		return;
	}

	// �ð�޾ڸ�Scene�ɉ�ʑJ�ڂ���B
	auto stageSelectScene = StageSelectScene::createStageSelectScene();
	// 0.5�b�����ăz���C�g�A�E�g���ă^�C�g���Ɉړ�����
	auto* fade = TransitionFadeUp::create(1.0f, stageSelectScene);
	// PauseLayer��j������titleScene�ɑJ�ڂ���
	Director::getInstance()->replaceScene(fade);

	_menuFlag = false;
}