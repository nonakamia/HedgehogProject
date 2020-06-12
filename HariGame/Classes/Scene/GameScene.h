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

#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "BaseScene.h"

#include "Obj/Obj.h"
#include "HP/HPMng.h"
#include "Action/ActionConvey.h"


class GameScene : public BaseScene
{
public:
    static cocos2d::Scene* createGameScene(std::string map);

    GameScene();
    ~GameScene();

    virtual bool init();

    void update(float delta);
    
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);

    void Resume();                          // ｹﾞｰﾑ再開

    bool GetGoalFlag();

    static void SetMap(std::string mapName);
    static std::string GetMapName();
    
private:
    // implement the "static create()" method manually
    CREATE_FUNC(GameScene);

    int _maxHP;

    cocos2d::Layer* _plauerLayer;
    cocos2d::Layer* _obstaclesLayer;
    Obj* _player_front;
    Obj* _player_behind;
    ActionConvey* _actionConvey;           // ｱｸｼｮﾝ伝達


    cocos2d::Action* _startAction;          // ｽﾀｰﾄ時のｱｸｼｮﾝ

    bool _goalFlag;

    cocos2d::Action* _gameOverAction;       // ｹﾞｰﾑｵｰﾊﾞｰ時のｱｸｼｮﾝ
    bool _gameOverFlag;

    ACTION _playerAction;
    HPMng* _hpMng;

    cocos2d::TMXTiledMap* _mapData;          // ﾏｯﾌﾟﾃﾞｰﾀ
    static std::string _mapName;

    cocos2d::MenuItemImage* _button;		// ﾒﾆｭｰﾎﾞﾀﾝ

    //@cricket
    CkSound* _gameSound;
    CkBank* _effectBank;
    CkSound* _clearSE;                      // ｸﾘｱ音
    CkSound* _failSE;                       // 失敗音

    bool GameStart();
    void GameOverAction();

    void AddClearLayer();                   // ｸﾘｱ画面表示

    void changeScene(Ref* pSender);         // StageSelectSceneに移動

    void SetMenu(Ref* pSender);             // ﾒﾆｭｰを開く

   
};

#endif // __HELLOWORLD_SCENE_H__
