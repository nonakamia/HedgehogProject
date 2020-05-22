/****************************************************************************
 Copyright (c) 2017-2018 Xiamen Yaji Software Co., Ltd.
 
 http://www.cocos2d-x.org
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#include "GameScene.h"
#include "Scene/BaseScene.h"
#include "Obj/Player/Player.h"
#include "Obj/Obstacles/Obstacles.h"
#include "Obj/Obstacles/BlackLadybug/BlackLadybug.h"
#include "Obj/Obstacles/Buds/Buds.h"
#include "Button/ButtonLayer.h"
#include "Camera/CameraOBJ.h"

//#include "SimpleAudioEngine.h"

USING_NS_CC;

Scene* GameScene::createGameScene()
{
    return GameScene::create();
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in GameScene.cpp\n");
}

// on "init" you need to initialize your instance
bool GameScene::init()
{
    if ( !Scene::init() )
    {
        return false;
    }
    setName("GameScnene");

    const Size visibleSize = Director::getInstance()->getVisibleSize();
    const Vec2 origin = Director::getInstance()->getVisibleOrigin();

    // 背景
    auto bgLayer = Layer::create();
    bgLayer->setName("BG_LAYER");
    this->addChild(bgLayer, static_cast<int>(zOlder::BG));

    // map読み込み
    _mapData = TMXTiledMap::create("stage/stage_0.tmx");
    _mapData->setName("MapData");
    bgLayer->addChild(_mapData);

    // OBSTACLES_LAYER
    _obstaclesLayer = Layer::create();
    _obstaclesLayer->setName("OBSTACLES_LAYER");
    this->addChild(_obstaclesLayer, static_cast<int>(zOlder::OBSTACLES));

    // 黒いてんとう虫  
    AddBlackLadybug();

    // PLAYER_LAYER
    _plauerLayer = Layer::create();
    _plauerLayer->setName("PLAYER_LAYER");
    this->addChild(_plauerLayer, static_cast<int>(zOlder::PLAYER));

    // player
    _player_front = Player::createPlayer(OBJ_COLOR::OBJ_RED);
    _player_front->setName("player_front");
    ((Player*)_player_front)->SetStartPosition(_mapData->getLayer("startPosition"), _mapData->getTileSize());
    _player_front->setScale(0.2f);
    _player_front->SetPoint(cocos2d::Vec2(20.0f, 20.0f));
    _player_front->scheduleUpdate();

    _player_behind = Player::createPlayer(OBJ_COLOR::OBJ_GREEN);
    _player_behind->setName("player_behind");
    _player_behind->setPosition(_player_front->getPosition());
    _player_behind->setScale(0.2f);
    _player_behind->SetPoint(cocos2d::Vec2(20.0f, 20.0f));
    _player_behind->scheduleUpdate();

    _plauerLayer->addChild(_player_behind);
    _plauerLayer->addChild(_player_front);

    // ｶﾒﾗ
    auto uiCamera = CameraOBJ();
    uiCamera(CameraFlag::USER1, this);
    getDefaultCamera()->setPosition(Vec2(visibleSize.width / 2.0f, (_mapData->getMapSize().height-6)*48));

    // ﾎﾞﾀﾝ
    auto buttonLayer = ButtonLayer::createButtonLayer();
    buttonLayer->setCameraMask(static_cast<int>(CameraFlag::USER1));
    this->addChild(buttonLayer, static_cast<int>(zOlder::BUTTON));

    // HP
   _hpMng = HPMng::createHPMng(3);
   _hpMng->setName("HP");
    _hpMng->setCameraMask(static_cast<int>(CameraFlag::USER1));
    this->addChild(_hpMng, static_cast<int>(zOlder::HP));

    _goalFlag = false;
    _gameOverFlag = false;
    _coolTimeAction = nullptr;
    _playerAction = ACTION::NON;

    this->scheduleUpdate();

    // TitleSceneに移動予定
    auto listener = EventListenerKeyboard::create();
    listener->onKeyReleased = [this](cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event)->bool
    {
        // ESCで終了(androidでは戻るﾎﾞﾀﾝと連動)
        if (keyCode == cocos2d::EventKeyboard::KeyCode::KEY_ESCAPE)
        {
            Director::getInstance()->end();
        }
        return true;
    };
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);

    return true;
}

void GameScene::update(float delta)
{
    if (_hpMng->GetHP() <= 0)
    {
        GameOverAction();
    }

    ActionConvey();

    if (((Player*)_player_front)->GetAction() == ACTION::NON)
    {
        ((Player*)_player_front)->SetAction(ACTION::ROTATE);
        SetActionConvey(ACTION::ROTATE);
    }

    // 同じ色との当たり判定
    auto playerPoint = ((Player*)_player_front)->GetPoint();
    if (!_goalFlag)
    {
        for (auto obj : _obstaclesLayer->getChildren())
        {
            if (obj->getName() == "lamp")
            {
                if (_player_front->getPositionX() >= obj->getPositionX())
                {
                    _goalFlag = true;
                }
            }

            if (((obj->getName() == "blackLadydug") || (obj->getName() == "buds")) && (!((BlackLadybug*)obj)->GetDamageFlag()))
            {
                ((Obstacles*)obj)->HitCheck(_plauerLayer, _hpMng);
            }
        }
    }
    else
    {
        auto winSize = Director::getInstance()->getWinSize();
        if (_player_behind->getPositionX() - _player_behind->GetPoint().x > getDefaultCamera()->getPositionX()+ winSize.width / 2.0f)
        {
            unscheduleUpdate();
            // playerのGameOverAction
            for (auto player : _plauerLayer->getChildren())
            {
                ((Obj*)player)->GameOverAction();
            }
        }
    }

    // ｶﾒﾗ
    auto winSize = Director::getInstance()->getWinSize();
    if ((!_goalFlag) && (_player_front->getPositionX() >= winSize.width / 2.0f)&&(getDefaultCamera()->getPositionX() < _player_front->getPositionX()))
    {
        getDefaultCamera()->setPositionX(_player_front->getPositionX());
    }
}


void GameScene::menuCloseCallback(Ref* pSender)
{
    //Close the cocos2d-x game scene and quit the application
    Director::getInstance()->end();

    /*To navigate back to native iOS screen(if present) without quitting the application  ,do not use Director::getInstance()->end() as given above,instead trigger a custom event created in RootViewController.mm as below*/

    //EventCustom customEndEvent("game_scene_close_event");
    //_eventDispatcher->dispatchEvent(&customEndEvent);


}

void GameScene::SetActionConvey(ACTION action)
{
    _playerAction = action;
    _coolTimeAction = runAction(DelayTime::create(0.6f));
    CC_SAFE_RETAIN(_coolTimeAction);
}

void GameScene::AddBlackLadybug()
{
    const Size visibleSize = Director::getInstance()->getVisibleSize();

    auto obstaclesLayer = _mapData->getLayer("obstacles");

    if (obstaclesLayer == nullptr)
    {
        return;
    }
    int chipSize = 48;
    auto mapSize = obstaclesLayer->getLayerSize();
    for (int y = 0; y < mapSize.height; y++)
    {
        for (int x = 0; x < mapSize.width; x++)
        {
            auto obstaclesPoint = Vec2{ (float)x,(float)y };
            auto obstaclesGid = obstaclesLayer->getTileGIDAt(obstaclesPoint);

            auto putPos = Vec2(obstaclesPoint.x * chipSize + (chipSize / 2), (mapSize.height - obstaclesPoint.y) * chipSize - (chipSize / 2));

            // 黒いてんとう虫
            if (obstaclesGid == OBSTACLES::LADYBUG_R)
            {
                auto blackLadydug = BlackLadybug::createBlackLadybug(OBJ_COLOR::OBJ_RED);
                blackLadydug->setName("blackLadydug");
                _obstaclesLayer->addChild(blackLadydug);
                blackLadydug->setPosition(putPos);
                blackLadydug->setScale(0.2f);
                blackLadydug->SetPoint(Vec2(20.0f, 20.0f));
            }
            if (obstaclesGid == OBSTACLES::LADYBUG_G)
            {
                auto blackLadydug = BlackLadybug::createBlackLadybug(OBJ_COLOR::OBJ_GREEN);
                blackLadydug->setName("blackLadydug");
                _obstaclesLayer->addChild(blackLadydug);
                blackLadydug->setPosition(putPos);
                blackLadydug->setScale(0.2f);
                blackLadydug->SetPoint(Vec2(20.0f, 20.0f));
            }

            // ﾂﾎﾞﾐ
            if (obstaclesGid == OBSTACLES::BUDS_R)
            {
                auto buds = Buds::createBuds(OBJ_COLOR::OBJ_RED);
                buds->setName("buds");
                _obstaclesLayer->addChild(buds);
                buds->setPosition(putPos);
                buds->setScale(0.5f);
                buds->SetPoint(Vec2(50.0f, 20.0f));
                buds->scheduleUpdate();
            }
            if (obstaclesGid == OBSTACLES::BUDS_G)
            {
                auto buds = Buds::createBuds(OBJ_COLOR::OBJ_GREEN);
                buds->setName("buds");
                _obstaclesLayer->addChild(buds);
                buds->setPosition(putPos);
                buds->setScale(0.5f);
                buds->SetPoint(Vec2(50.0f, 20.0f));
                buds->scheduleUpdate();
            }

            // ﾗﾝﾌﾟ
            if (obstaclesGid == OBSTACLES::LAMP)
            {
                auto lamp = Sprite::create("Ornament/lamp.png");
                lamp->setName("lamp");
                _obstaclesLayer->addChild(lamp);
                lamp->setScale(0.8f);
                lamp->setAnchorPoint(Point(0.5f, 0.0f));
                lamp->setPosition(Vec2(putPos.x, putPos.y - (_mapData->getTileSize().height / 2.0f)));
            }
        }
    }
}

void GameScene::GameOverAction()
{
    _gameOverFlag = true;
    unscheduleUpdate();
    
    // playerのGameOverAction
    for (auto player : _plauerLayer->getChildren())
    {
        ((Obj*)player)->GameOverAction();
    }

}

void GameScene::ActionConvey()
{
    if (_coolTimeAction == nullptr)
    {
        return;
   }

    if (_coolTimeAction->isDone())
    {
         CC_SAFE_RELEASE_NULL(_coolTimeAction);
        ((Player*)_player_behind)->SetAction(_playerAction);
    }
}
