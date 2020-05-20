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
#include "Obj/BlackLadybug/BlackLadybug.h"
#include "Button/ButtonLayer.h"
#include "Camera/CameraOBJ.h"
#include "HP/HPMng.h"
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
    //////////////////////////////
    // 1. super init first
    if ( !Scene::init() )
    {
        return false;
    }
    setName("GameScnene");

    const Size visibleSize = Director::getInstance()->getVisibleSize();
    const Vec2 origin = Director::getInstance()->getVisibleOrigin();

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    auto closeItem = MenuItemImage::create(
                                           "CloseNormal.png",
                                           "CloseSelected.png",
                                           CC_CALLBACK_1(GameScene::menuCloseCallback, this));

    if (closeItem == nullptr ||
        closeItem->getContentSize().width <= 0 ||
        closeItem->getContentSize().height <= 0)
    {
        problemLoading("'CloseNormal.png' and 'CloseSelected.png'");
    }
    else
    {
        float x = origin.x + visibleSize.width - closeItem->getContentSize().width/2;
        float y = origin.y + closeItem->getContentSize().height/2;
        closeItem->setPosition(Vec2(x,y));
    }

    // create menu, it's an autorelease object
    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Vec2::ZERO);
    menu->setCameraMask(static_cast<int>(CameraFlag::USER1));
    this->addChild(menu, 1);

    /////////////////////////////
    // 3. add your codes below...

    // add a label shows "Hello World"
    // create and initialize a label

    auto label = Label::createWithTTF("Hello World", "fonts/Marker Felt.ttf", 24);
    if (label == nullptr)
    {
        problemLoading("'fonts/Marker Felt.ttf'");
    }
    else
    {
        // position the label on the center of the screen
        label->setPosition(Vec2(origin.x + visibleSize.width/2,
                                origin.y + visibleSize.height - label->getContentSize().height));

        // add the label as a child to this layer
        this->addChild(label, 1);
    }

    // ”wŒi
    auto bgLayer = Layer::create();
    bgLayer->setName("BG_LAYER");
    this->addChild(bgLayer, static_cast<int>(zOlder::BG));

    // map“Ç‚Ýž‚Ý
    _mapData = TMXTiledMap::create("stage/stage_0.tmx");
    _mapData->setName("MapData");
    bgLayer->addChild(_mapData);

    // OBJ_LAYER
    _objLayer = Layer::create();
    _objLayer->setName("OBJ_LAYER");
    this->addChild(_objLayer, static_cast<int>(zOlder::CHAR));

    // •‚¢‚Ä‚ñ‚Æ‚¤’Ž  
    AddBlackLadybug();

    // player
    _player_front = Player::createPlayer(OBJ_COLOR::OBJ_RED, cocos2d::Vec2(20.0f, 20.0f));
    _player_front->setName("player_front");
    ((Player*)_player_front)->SetStartPosition(_mapData->getLayer("startPosition"), _mapData->getTileSize());
    _player_front->setScale(0.2f);
    _player_front->scheduleUpdate();

    _player_behind = Player::createPlayer(OBJ_COLOR::OBJ_GREEN, cocos2d::Vec2(20.0f, 20.0f));
    _player_behind->setName("player_behind");
    _player_behind->setPosition(_player_front->getPosition());
    _player_behind->setScale(0.2f);
    _player_behind->scheduleUpdate();

    _objLayer->addChild(_player_behind);
    _objLayer->addChild(_player_front);

    // ¶Ò×
    auto uiCamera = CameraOBJ();
    uiCamera(CameraFlag::USER1, this);
    getDefaultCamera()->setPosition(Vec2(origin.x + visibleSize.width / 2.0f, origin.y + (_mapData->getMapSize().height-6)*48));

    // ÎÞÀÝ
    auto buttonLayer = ButtonLayer::createButtonLayer();
    buttonLayer->setCameraMask(static_cast<int>(CameraFlag::USER1));
    this->addChild(buttonLayer, static_cast<int>(zOlder::BUTTON));

    // HP
    auto hpMng = HPMng::createHPMng(3);
    hpMng->setCameraMask(static_cast<int>(CameraFlag::USER1));
    this->addChild(hpMng, static_cast<int>(zOlder::BUTTON));

    _goalFlag = false;
    _coolTimeAction = nullptr;
    _playerAction = ACTION::NON;

    this->scheduleUpdate();

    // ÃÞÊÞ¯¸—p
#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
    auto listener = EventListenerKeyboard::create();
    listener->onKeyReleased = [this](cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event)->bool
    {
        // ESC‚ÅI—¹
        if (keyCode == cocos2d::EventKeyboard::KeyCode::KEY_ESCAPE)
        {
            Director::getInstance()->end();
        }
        return true;
    };
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
#endif
    return true;
}

void GameScene::update(float delta)
{
    ActionConvey();

    if (((Player*)_player_front)->GetAction() == ACTION::NON)
    {
        ((Player*)_player_front)->SetAction(ACTION::ROTATE);
        SetActionConvey(ACTION::ROTATE);
    }

    // “¯‚¶F‚Æ‚Ì“–‚½‚è”»’è
    auto playerPoint = ((Player*)_player_front)->getPoint();
    if (!_goalFlag)
    {
        for (auto obj : _objLayer->getChildren())
        {

            if (obj->getName() == "blackLadydug")
            {
                if ((_player_front->getPosition().x - playerPoint.x <= obj->getPosition().x) &&
                    (_player_front->getPosition().x + playerPoint.x >= obj->getPosition().x) &&
                    (_player_front->getPosition().y - playerPoint.y <= obj->getPosition().y) &&
                    (_player_front->getPosition().y + playerPoint.y >= obj->getPosition().y))
                {
                    if (_player_front->getTag() == obj->getTag())
                    {
                        ((BlackLadybug*)obj)->HitAction();
                    }
                    else
                    {
                        ((Player*)_player_front)->DamageAction();
                        ((Player*)_player_behind)->DamageAction();
                    }
                }
            }

            if (obj->getName() == "lamp")
            {
                if (_player_front->getPositionX() >= obj->getPositionX())
                {
                    _goalFlag = true;
                }
            }
        }
    }

    // ¶Ò×
    auto winSize = Director::getInstance()->getWinSize();
    if ((!_goalFlag)&&(_player_front->getPositionX() >= winSize.width / 2.0f))
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
    const Vec2 origin = Director::getInstance()->getVisibleOrigin();

    auto obstaclesLayer = _mapData->getLayer("obstacles");

    int chipSize = 48;
    auto mapSize = obstaclesLayer->getLayerSize();
    for (int y = 0; y < mapSize.height; y++)
    {
        for (int x = 0; x < mapSize.width; x++)
        {
            auto obstaclesPoint = Vec2{ (float)x,(float)y };
            auto obstaclesGid = obstaclesLayer->getTileGIDAt(obstaclesPoint);

            auto putPos = Vec2(obstaclesPoint.x * chipSize + (chipSize / 2) + origin.x, (mapSize.height - obstaclesPoint.y) * chipSize - (chipSize / 2) + origin.y);

            if (obstaclesGid == OBSTACLES::LADYBUG_R)
            {
                auto blackLadydug = BlackLadybug::createBlackLadybug(OBJ_COLOR::OBJ_RED);
                blackLadydug->setName("blackLadydug");
                _objLayer->addChild(blackLadydug);
                blackLadydug->setPosition(putPos);
                blackLadydug->setScale(0.2f);
            }
            if (obstaclesGid == OBSTACLES::LADYBUG_G)
            {
                auto blackLadydug = BlackLadybug::createBlackLadybug(OBJ_COLOR::OBJ_GREEN);
                blackLadydug->setName("blackLadydug");
                _objLayer->addChild(blackLadydug);
                blackLadydug->setPosition(putPos);
                blackLadydug->setScale(0.2f);
            }
            if (obstaclesGid == OBSTACLES::LAMP)
            {
                auto lamp = Sprite::create("Ornament/lamp.png");
                lamp->setName("lamp");
                _objLayer->addChild(lamp);
                lamp->setScale(0.8f);
                lamp->setAnchorPoint(Point(0.5f, 0.0f));
                lamp->setPosition(Vec2(putPos.x, putPos.y - (_mapData->getTileSize().height / 2.0f)));
            }
        }
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
