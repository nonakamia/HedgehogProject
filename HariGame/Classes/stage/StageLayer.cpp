#include "StageLayer.h"
#include "Scene/StageSelectScene.h"

USING_NS_CC;

Layer* StageLayer::createStageLayer(StageData data)
{
	StageLayer* pRet = new(std::nothrow) StageLayer(data);
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

StageLayer::StageLayer(StageData data)
{
	_stageData = data;
	_minimumLayerPosX = 0.0f;
	_calloutFlag = false;
	_selectFlag = false;
	_rank = 0;

	//@cricket
	_buttonBank = nullptr;
	_buttonSE = nullptr;
}

StageLayer::~StageLayer()
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

bool StageLayer::init()
{
	if (!Layer::init())
	{
		return false;
	}

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	// UserDefault�Ăяo��
	UserDefault* _userDef = UserDefault::getInstance();
	// �܂����݂��Ă��Ȃ��ꍇ����̫�Ēl(0)������
	_rank = _userDef->getIntegerForKey(_stageData.stageName.c_str());

	auto stage = Sprite::create("StageSelect/stage.png");
	addChild(stage);
	stage->setName("stage");
	stage->setPosition(Vec2(
		origin.x + visibleSize.width / 2.0f,
		origin.y + visibleSize.height / 4.0f
	));
	auto changeListener = EventListenerTouchOneByOne::create();
	// �����ꂽ���̏���
	changeListener->onTouchBegan = [this, visibleSize, stage](Touch* touch, Event* event)
	{
		// layer���̂����炵�Ă��邽�߁Athis->getPosition()�ŏC��
		Vec2 point = touch->getLocation() - this->getPosition();
		Rect rectButton = stage->getBoundingBox();
		if (rectButton.containsPoint(point))
		{
			return true;
		}
		else
		{
			for (auto child : this->getChildren())
			{
				if (child->getName() != "stage")
				{
					child->runAction(ScaleTo::create(0.1f, 0.0f));
				}
			}
			_calloutFlag = false;
		}
		return false;
	};
	// �����ꂽ���̏���
	changeListener->onTouchEnded = [this](Touch* touch, Event* event)
	{

		//@cricket
		_buttonSE->play();

		if (!_calloutFlag)
		{
			for (auto child : this->getChildren())
			{
				if (child->getName() != "stage")
				{
					child->runAction(ScaleTo::create(0.1f, 1.0f));
				}
			}
			_calloutFlag = true;
		}
		else
		{
			_selectFlag = true;
		}
		return true;
	};
	getEventDispatcher()->addEventListenerWithSceneGraphPriority(changeListener, this);

	// �����o��
	_callout = Sprite::create(_stageData.imagePass);
	_callout->setName("callout");
	addChild(_callout);
	_callout->setAnchorPoint(Point(0.5f, 0.0f));
	_callout->setPosition(Vec2(
		stage->getPosition().x,
		origin.y + visibleSize.height / 2.0f
	));
	_callout->setScale(0.0f);

	// �ݸ
	if (_stageData.stageName != "NoStage")
	{
		for (int i = 1; i <= 3; i++)
		{
			auto rankSprite = Sprite::create("rank/star_hidden.png");
			addChild(rankSprite);
			if (_rank >= i)
			{
				// �ݸ��1�ȏ�Ȃ�\������
				rankSprite->setSpriteFrame(Sprite::create("rank/star.png")->getSpriteFrame());
			}

			rankSprite->setPosition(
				Vec2(_callout->getPosition().x + (rankSprite->getContentSize().width * (i - 2)),
					_callout->getPosition().y + (_callout->getContentSize().height / 2)
				)
			);
			rankSprite->setScale(0.0f);
		}
	}

	//@cricket
#ifdef CK_PLATFORM_WIN
	_buttonBank = CkBank::newBank("Resources/se/button/button.ckb");
#else
	_buttonBank = CkBank::newBank("se/button/button.ckb");
#endif
	_buttonSE = CkSound::newBankSound(_buttonBank, "decision");

	return true;
}

void StageLayer::update(float delta)
{
	// �ð�ނ��ȏ㓯���������ꂽ�ꍇ
	// �E�̽ð�ނ�D��ɂ���
	for (auto obj : Director::getInstance()->getRunningScene()->getChildren())
	{
		if ((obj->getName() == "stage") && (((StageLayer*)obj)->GetCalloutFlag()) && (((StageLayer*)obj)->GetMinimumLayerPosX() > _minimumLayerPosX))
		{
			for (auto child : this->getChildren())
			{
				if (child->getName() != "stage")
				{
					child->setScale(0.0f);
				}
			}
			_calloutFlag = false;
			break;
		}
	}

	if ((_buttonSE) && (!_buttonSE->isPlaying()) && (_selectFlag))
	{
		auto scene = Director::getInstance()->getRunningScene();
		if (((StageSelectScene*)scene)->changeScene(this, _stageData.stageName, _stageData.mapData))
		{
			_buttonBank->destroy();
			_buttonSE->destroy();
			_buttonBank = nullptr;
			_buttonSE = nullptr;
		}
	}
}

void StageLayer::SetMinimumLayerPosX(float posX)
{
	_minimumLayerPosX = posX;
}

float StageLayer::GetMinimumLayerPosX()
{
	return _minimumLayerPosX;
}

bool StageLayer::GetCalloutFlag()
{
	return _calloutFlag;
}
