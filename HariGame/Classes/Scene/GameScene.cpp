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
#include "BaseScene.h"
#include "StageSelectScene.h"
#include "ClearLayer/ClearLayer.h"
#include "Obj/Player/Player.h"
#include "Obj/Obstacles/Obstacles.h"
#include "Obj/Obstacles/AddObstacles.h"
#include "Button/ButtonLayer.h"
#include "Camera/CameraOBJ.h"
#include "menu/MenuLayer.h"
#include "RankCheck/RankCheck.h"
#include "Split/Split.h"

USING_NS_CC;

std::string GameScene::_mapPass;
std::string GameScene::_stageName;

Scene* GameScene::createGameScene(std::string name, std::string map)
{
    GameScene* pRet = new(std::nothrow) GameScene(name, map);
    if (pRet->init())
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
    return GameScene::create();
}

GameScene::GameScene(std::string name, std::string map)
{
    _stageName = name;
    _mapPass = map;

    _changeSceneFlag = false;
    _menuFlag = false;

    //@cricket
    _gameSound = nullptr;
    _effectBank = nullptr;
    _clearSE = nullptr;
    _failSE = nullptr;
}

GameScene::~GameScene()
{
    // ƒV[ƒ“Ø‘ÖŽž‚É~ƒ{ƒ^ƒ“‰Ÿ‚µ‚½‚ç‚Á”ò‚Ô‚Ì‚ð–h‚®
    if (_running)
    {
        onExit();
    }

    //@cricket
    if (_gameSound)
    {
        _gameSound->destroy();
        _gameSound = nullptr;
    }
    if (_effectBank)
    {
        _effectBank->destroy();
        _effectBank = nullptr;
    }
    if (_clearSE)
    {
        _clearSE->destroy();
        _clearSE = nullptr;
    }
    if (_failSE)
    {
        _failSE->destroy();
        _failSE = nullptr;
    }
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
    setName("GameScene");

    // UserDefaultŒÄ‚Ño‚µ
    UserDefault* _userDef = UserDefault::getInstance();
    // ÃÞÌ«ÙÄ‚Ì‚Ü‚Ü‚Å‚È‚¢‚©
    if (_userDef->getIntegerForKey("MAX_HP") != 0)
    {
        _maxHP = _userDef->getIntegerForKey("MAX_HP");
    }
    else
    {
        // ŠO•”ÃÞ°À“Ç‚Ýž‚Ý
        std::string defaultIfs = FileUtils::getInstance()->getStringFromFile("csv/setting_default.csv");
        if (defaultIfs == "")
        {
            return false;
        }
        ValueVector defaultCsvSplit = Split::split(defaultIfs, "\n");
        for (int i = 1; i < (int)defaultCsvSplit.size(); i++)
        {
            ValueVector defaultCsvData = Split::split(defaultCsvSplit.at(i).asString(), ",");
            _maxHP = defaultCsvData.at(0).asInt();
        }
        // ‘‚«ž‚Ý
        _userDef->setIntegerForKey("MAX_HP", _maxHP);
    }
   

    const Size visibleSize = Director::getInstance()->getVisibleSize();
    const Vec2 origin = Director::getInstance()->getVisibleOrigin();

    // ”wŒi
    auto bgLayer = Layer::create();
    bgLayer->setName("BG_LAYER");
    this->addChild(bgLayer, static_cast<int>(zOlder::BG));

    // map“Ç‚Ýž‚Ý
    _mapData = TMXTiledMap::create(_mapPass);
    _mapData->setName("MapData");
    bgLayer->addChild(_mapData);

    // OBSTACLES_LAYER
    _obstaclesLayer = Layer::create();
    _obstaclesLayer->setName("OBSTACLES_LAYER");
    this->addChild(_obstaclesLayer, static_cast<int>(zOlder::OBSTACLES));
    // áŠQ•¨  
    auto addObstacles = AddObstacles();
    addObstacles(_obstaclesLayer, _mapData->getLayer("obstacles"));

    // PLAYER_LAYER
    _plauerLayer = Layer::create();
    _plauerLayer->setName("PLAYER_LAYER");
    this->addChild(_plauerLayer, static_cast<int>(zOlder::PLAYER));

    // player
    _player_front = Player::createPlayer(OBJ_COLOR::OBJ_RED);
    _player_front->setName("player_front");
    // À•WŽw’è
    auto startPos = Vec2(_userDef->getFloatForKey("C_POINT_X"), _userDef->getFloatForKey("C_POINT_Y"));
    if (startPos != Vec2::ZERO)
    {
        getDefaultCamera()->setPosition(startPos.x, getDefaultCamera()->getPosition().y);
        // Áª¯¸Îß²ÝÄ‚©‚ç
        _player_front->setPosition(startPos);
    }
    else
    {
        // Tiled‚ÅŽw’è‚µ‚½À•W‚ÉÝ’u‚·‚é        
        ((Player*)_player_front)->SetStartPosition(_mapData->getLayer("startPosition"), _mapData->getTileSize());
    }

    _player_front->SetPoint(cocos2d::Vec2(20.0f, 20.0f));

    _player_behind = Player::createPlayer(OBJ_COLOR::OBJ_GREEN);
    _player_behind->setName("player_behind");
    _player_behind->setPosition(_player_front->getPosition());
    _player_behind->SetPoint(cocos2d::Vec2(20.0f, 20.0f));

    _plauerLayer->addChild(_player_behind);
    _plauerLayer->addChild(_player_front);

    // ¶Ò×
    auto uiCamera = CameraOBJ();
    uiCamera(CameraFlag::USER1, this);
    //getDefaultCamera()->setPosition(Vec2(visibleSize.width / 2.0f, (_mapData->getMapSize().height-6)*48));
    getDefaultCamera()->setPosition(getDefaultCamera()->getPosition().x, (_mapData->getMapSize().height - 6) * 48);

    // HP
   _hpMng = HPMng::createHPMng(_maxHP);
   _hpMng->setName("HP");
    _hpMng->setCameraMask(static_cast<int>(CameraFlag::USER1));
    this->addChild(_hpMng, static_cast<int>(zOlder::HP));

    // ÒÆ­°ÎÞÀÝ
    _button = MenuItemImage::create(
        "menu/menuButton.png",
        "menu/menuButton.png",
        CC_CALLBACK_1(GameScene::SetMenu, this));
    _button->setPosition(Vec2(origin.x + 50.0f,
        origin.y + visibleSize.height - 50.0f));
    auto menu = Menu::create(_button, nullptr);
    menu->setPosition(Vec2::ZERO);
    menu->setCameraMask(static_cast<int>(CameraFlag::USER1));
    addChild(menu, static_cast<int>(zOlder::BUTTON));

    _startAction = runAction(DelayTime::create(3.0f));
    CC_SAFE_RETAIN(_startAction);

    _gameOverAction = nullptr;

    _goalFlag = false;
    _gameOverFlag = false;
    _playerAction = ACTION::NON;

    //@cricket
#ifdef CK_PLATFORM_WIN
    _gameSound = CkSound::newStreamSound("Resources/sound/CabalHill.cks");
    _effectBank = CkBank::newBank("Resources/se/effect/effect.ckb");
#else
    _gameSound = CkSound::newStreamSound("sound/CabalHill.cks");
    _effectBank = CkBank::newBank("se/effect/effect.ckb");
#endif
    _gameSound->setLoopCount(-1);
    _gameSound->setVolume(0.5f);
    _clearSE = CkSound::newBankSound(_effectBank, "clear");
    _failSE = CkSound::newBankSound(_effectBank, "fail");

    this->scheduleUpdate();

    return true;
}

void GameScene::update(float delta)
{
    getDefaultCamera()->setPosition(getDefaultCamera()->getPosition().x, (_mapData->getMapSize().height - 6) * 48);
    if (GameStart())
    {
        if (_hpMng->GetHP() <= 0)
        {
            GameOverAction();
        }

        // “¯‚¶F‚Æ‚Ì“–‚½‚è”»’è
        auto playerPoint = ((Player*)_player_front)->GetPoint();
        if (!_goalFlag)
        {
            for (auto obj : _obstaclesLayer->getChildren())
            {
                if (obj->getName() == "lamp")
                {
                    if (_player_front->getPosition().x >= obj->getPosition().x)
                    {
                        RankCheck::rankCheck(_stageName, _hpMng->GetHP());
                        AddClearLayer();
                        _goalFlag = true;
                    }
                }
                else if ((!((Obstacles*)obj)->GetDamageFlag()))
                {
                    ((Obstacles*)obj)->HitCheck(_plauerLayer, _hpMng);
                }
            }
        }
        else
        {
          
            auto winSize = Director::getInstance()->getWinSize();
            if (_player_behind->getPosition().x - _player_behind->GetPoint().x > getDefaultCamera()->getPosition().x + winSize.width / 2.0f)
            {
                for (auto player : _plauerLayer->getChildren())
                {
                    ((Obj*)player)->GameClearAction();
                }
            }
        }

        // ¶Ò×
        auto winSize = Director::getInstance()->getWinSize();
        if ((!_goalFlag) && (_player_front->getPosition().x >= winSize.width / 2.0f) && (getDefaultCamera()->getPosition().x < _player_front->getPosition().x))
        {
            getDefaultCamera()->setPosition(_player_front->getPosition().x, getDefaultCamera()->getPosition().y);
        }
    }

    //@cricket
    CkUpdate();
}


void GameScene::menuCloseCallback(Ref* pSender)
{
    //Close the cocos2d-x game scene and quit the application
    Director::getInstance()->end();

    /*To navigate back to native iOS screen(if present) without quitting the application  ,do not use Director::getInstance()->end() as given above,instead trigger a custom event created in RootViewController.mm as below*/

    //EventCustom customEndEvent("game_scene_close_event");
    //_eventDispatcher->dispatchEvent(&customEndEvent);


}

bool GameScene::GameStart()
{
    if (_startAction == nullptr)
    {
        return true;
    }

    if (_startAction->isDone())
    {
        CC_SAFE_RELEASE_NULL(_startAction);

        _player_front->scheduleUpdate();
        _player_behind->scheduleUpdate();

        // ±¸¼®Ý“`’B
        _actionConvey = ActionConvey::createActionConvey();
        _actionConvey->setName("actionConvey");
        this->addChild(_actionConvey);
        _actionConvey->scheduleUpdate();

        //“]‚ª‚è
        ((Player*)_player_front)->SetAction(ACTION::ROTATE);
        _actionConvey->SetActionConvey(ACTION::ROTATE);

        // ÎÞÀÝ
        auto buttonLayer = ButtonLayer::createButtonLayer();
        buttonLayer->setCameraMask(static_cast<int>(CameraFlag::USER1));
        this->addChild(buttonLayer, static_cast<int>(zOlder::BUTTON));

        //@cricket
        _gameSound->play();
        return true;
    }

    return false;
}

void GameScene::GameOverAction()
{
    if (!_gameOverFlag)
    {
        _gameOverFlag = true;

        _failSE->play();
        // player‚ÌGameOverAction
        for (auto player : _plauerLayer->getChildren())
        {
            ((Obj*)player)->GameOverAction();
        }
        _gameOverAction = runAction(DelayTime::create(1.5f));
        CC_SAFE_RETAIN(_gameOverAction);
        return;
    }

    if (_gameOverAction == nullptr)
    {
        return;
    }

    if (_gameOverAction->isDone())
    {
        CC_SAFE_RELEASE_NULL(_gameOverAction);
        changeScene(this);
    }

}

void GameScene::AddClearLayer()
{
    //@cricket
    _clearSE->play();

    auto clearLayer = ClearLayer::createClearLayer();
    clearLayer->setCameraMask(static_cast<int>(CameraFlag::USER1));
    this->addChild(clearLayer, static_cast<int>(zOlder::MENU));
}

void GameScene::changeScene(Ref* pSender)
{
    if (_changeSceneFlag)
    {
        return;
    }

    if (!_changeSceneFlag)
    {
        _gameSound->stop();

        unscheduleUpdate();

        //// ƒZƒŒƒNƒgƒV[ƒ“‚É‰æ–Ê‘JˆÚ‚·‚éB
        //auto stageSelectScene = StageSelectScene::createStageSelectScene();
        //auto* fade = TransitionFade::create(1.0f, stageSelectScene, Color3B::BLACK);
        //// GameScene‚ð”jŠü‚µ‚ÄStageSelectScene‚É‘JˆÚ‚·‚é
        //Director::getInstance()->replaceScene(fade);

        // ƒZƒŒƒNƒgƒV[ƒ“‚É‰æ–Ê‘JˆÚ‚·‚éB
        auto gameScene = GameScene::createGameScene(_stageName, _mapPass);
        auto* fade = TransitionFade::create(1.0f, gameScene, Color3B::BLACK);
        // GameScene‚ð”jŠü‚µ‚ÄStageSelectScene‚É‘JˆÚ‚·‚é
        Director::getInstance()->replaceScene(fade);

        //@cricket
        _gameSound->destroy();
        _effectBank->destroy();
        _clearSE->destroy();
        _failSE->destroy();
        _gameSound = nullptr;
        _effectBank = nullptr;
        _clearSE = nullptr;
        _failSE = nullptr;

        _changeSceneFlag = true;
    }
}

void GameScene::SetMenu(Ref* pSender)
{
    if ((_gameOverFlag)||(_goalFlag))
    {
        return;
    }

    if (!_menuFlag)
    {
        this->stopAllActions();
        this->unscheduleUpdate();

        for (auto player : _plauerLayer->getChildren())
        {
            player->stopAllActions();
            player->unscheduleUpdate();
        }

        auto menuLayer = MenuLayer::createMenuLayer();
        this->addChild(menuLayer, static_cast<int>(zOlder::MENU));
        menuLayer->setCameraMask(static_cast<int>(CameraFlag::USER1));

        _button->setVisible(false);
        _menuFlag = true;
    }
}

void GameScene::Resume()
{
    _menuFlag = false;
    this->scheduleUpdate();
    if (_startAction)
    {
       this->runAction(_startAction);
    }
    else
    {
        for (auto player : _plauerLayer->getChildren())
        {
            player->scheduleUpdate();
            ((Player*)player)->Resume();
        }
    }
    _button->setVisible(true);
}

bool GameScene::GetGoalFlag()
{
    return _goalFlag;
}

std::string GameScene::GetStageName()
{
    return _stageName;
}

std::string GameScene::GetMapPass()
{
    return _mapPass;
}
