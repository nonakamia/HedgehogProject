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
#include "Button/ButtonLayer.h"
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

    // add "HelloWorld" splash screen"
    //auto sprite = Sprite::create("HelloWorld.png");
    //if (sprite == nullptr)
    //{
    //    problemLoading("'HelloWorld.png'");
    //}
    //else
    //{
    //    // position the sprite on the center of the screen
    //    sprite->setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));

    //    // add the sprite as a child to this layer
    //    this->addChild(sprite, 0);
    //    sprite->runAction(RepeatForever::create(RotateBy::create(1.0f, 360.0f)));
    //}

    // �w�i
    auto bgLayer = Layer::create();
    bgLayer->setName("BG_LAYER");
    this->addChild(bgLayer, static_cast<int>(zOlder::BG));

    // map�ǂݍ���
    auto mapData = TMXTiledMap::create("stage/stage_0.tmx");
    mapData->setName("MapData");
    bgLayer->addChild(mapData);

    _scaffoldLayer = mapData->getLayer("scaffold");
    _scaffoldLayer->setName("Scaffold");


    // OBJ_LAYER
    auto objLayer = Layer::create();
    objLayer->setName("OBJ_LAYER");
    this->addChild(objLayer, static_cast<int>(zOlder::CHAR));
    // player
    auto playerSprit_front = Player::createPlayer(OBJ_COLOR::OBJ_RED, cocos2d::Vec2(20.0f,20.0f));
    playerSprit_front->setName("player_front");
    objLayer->addChild(playerSprit_front);
    playerSprit_front->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
    playerSprit_front->scheduleUpdate();

    auto playerSprit_behind = Player::createPlayer(OBJ_COLOR::OBJ_GREEN, cocos2d::Vec2(20.0f, 20.0f));
    playerSprit_behind->setName("player_behind");
    objLayer->addChild(playerSprit_behind);
    playerSprit_behind->setPosition(Vec2(visibleSize.width / 2 + origin.x - 100.0f, visibleSize.height / 2 + origin.y));
    playerSprit_behind->scheduleUpdate();

    // ����
    auto buttonLayer = ButtonLayer::createButtonLayer();
    this->addChild(buttonLayer, static_cast<int>(zOlder::BUTTON));

    // ���ޯ��p
#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
    auto listener = EventListenerKeyboard::create();
    listener->onKeyReleased = [this](cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event)->bool
    {
        // ESC�ŏI��
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


void GameScene::menuCloseCallback(Ref* pSender)
{
    //Close the cocos2d-x game scene and quit the application
    Director::getInstance()->end();

    /*To navigate back to native iOS screen(if present) without quitting the application  ,do not use Director::getInstance()->end() as given above,instead trigger a custom event created in RootViewController.mm as below*/

    //EventCustom customEndEvent("game_scene_close_event");
    //_eventDispatcher->dispatchEvent(&customEndEvent);


}
