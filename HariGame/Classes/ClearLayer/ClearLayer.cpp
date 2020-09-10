#include "ClearLayer.h"
#include "Scene/StageSelectScene.h"
#include "Scene/GameScene.h"

USING_NS_CC;

Layer* ClearLayer::createClearLayer()
{
	ClearLayer* pRet = new(std::nothrow) ClearLayer();
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

ClearLayer::ClearLayer()
{
	_visible = true;
}

ClearLayer::~ClearLayer()
{
}

bool ClearLayer::init()
{
	if (!Layer::init())
	{
		return false;
	}
	setName("ClearLayer");

	// UserDefault呼び出し
	UserDefault* _userDef = UserDefault::getInstance();

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto clearWaku = Sprite::create("Clear/clearWaku.png");
	addChild(clearWaku);
	clearWaku->setPosition(Vec2(
		origin.x + visibleSize.width / 2.0f,
		origin.y + visibleSize.height / 2.0f
	));
	clearWaku->setScale(0.0f);
	clearWaku->runAction(Sequence::create(
		DelayTime::create(1.0f),
		ScaleTo::create(0.2f, 1.0f),
		nullptr
	));

	// ｸﾘｱ
	auto clear = Sprite::create("Clear/clear.png");
	addChild(clear);
	clear->setPosition(Vec2(
		clearWaku->getPosition().x,
		clearWaku->getPosition().y * 1.3f
	));
	clear->setScale(0.0f);
	clear->runAction(Sequence::create(
		DelayTime::create(1.0f),
		ScaleTo::create(0.2f, 1.0f),
		nullptr
	));

	// ﾗﾝｸ
	int rank = _userDef->getIntegerForKey("nowRank");
	for (int i = 1; i <= 3; i++)
	{
		auto rankSprite = Sprite::create("rank/star_hidden.png");
		addChild(rankSprite);
		if (rank >= i)
		{
			// ﾗﾝｸが1以上なら表示する
			rankSprite->setSpriteFrame(Sprite::create("rank/star.png")->getSpriteFrame());
		}

		rankSprite->setPosition(
			Vec2(clearWaku->getPosition().x + ((rankSprite->getContentSize().width + 20.0f) * (i - 2)),
				clearWaku->getPosition().y + (rankSprite->getContentSize().height / 4.0f)
			)
		);
		rankSprite->setScale(0.0f);
		rankSprite->runAction(Sequence::create(
			DelayTime::create(1.0f),
			ScaleTo::create(0.2f, 1.0f),
			nullptr
		));
	}


	// ｽﾃｰｼﾞｾﾚｸﾄに戻る
	auto backStageSlect = MenuItemImage::create(
		"menu/backStageSelect.png",
		"menu/backStageSelect.png",
		CC_CALLBACK_1(ClearLayer::BackStageSelectScene, this));
	backStageSlect->setPosition(Vec2(
		clearWaku->getPosition().x,
		clearWaku->getPosition().y * 0.8f
	));
	backStageSlect->setScale(0.0f);
	backStageSlect->runAction(Sequence::create(
		DelayTime::create(1.0f),
		ScaleTo::create(0.2f, 0.7f),
		nullptr
	));
	auto backStageSlectMenu = Menu::create(backStageSlect, nullptr);
	backStageSlectMenu->setPosition(Vec2::ZERO);
	addChild(backStageSlectMenu);

	// はじめから
	auto fromScratch = MenuItemImage::create(
		"menu/fromScratch.png",
		"menu/fromScratch.png",
		CC_CALLBACK_1(ClearLayer::SromScratch, this));
	fromScratch->setPosition(Vec2(
		clearWaku->getPosition().x,
		clearWaku->getPosition().y * 0.6f
	));
	fromScratch->setScale(0.0f);
	fromScratch->runAction(Sequence::create(
		DelayTime::create(1.0f),
		ScaleTo::create(0.2f, 0.7f),
		nullptr
	));
	auto fromScratchMenu = Menu::create(fromScratch, nullptr);
	fromScratchMenu->setPosition(Vec2::ZERO);
	addChild(fromScratchMenu);


	return true;
}

void ClearLayer::BackStageSelectScene(Ref* pSender)
{
	if (!_visible)
	{
		return;
	}

	// ｽﾃｰｼﾞｾﾚｸﾄSceneに画面遷移する。
	auto stageSelectScene = StageSelectScene::createStageSelectScene();
	// 0.5秒かけてホワイトアウトしてタイトルに移動する
	auto* fade = TransitionFadeUp::create(1.0f, stageSelectScene);
	// PauseLayerを破棄してtitleSceneに遷移する
	Director::getInstance()->replaceScene(fade);

	_visible = false;
}

void ClearLayer::SromScratch(Ref* pSender)
{
	if (!_visible)
	{
		return;
	}

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

	_visible = false;
}