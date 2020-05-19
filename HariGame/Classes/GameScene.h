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

#include "Obj/Obj.h"

class GameScene : public cocos2d::Scene
{
public:
    static cocos2d::Scene* createGameScene();

    virtual bool init();

    void update(float delta);
    
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);

    void SetActionConvey(ACTION action);
    
private:
    // implement the "static create()" method manually
    CREATE_FUNC(GameScene);

    // ��Q��
    enum OBSTACLES
    {
        LADYBUG_R = 212,
        LADYBUG_G = 213,
        MAX
    };

    cocos2d::Layer* _objLayer;
    Obj* _player_front;
    Obj* _player_behind;

    cocos2d::Action* _coolTimeAction;
    cocos2d::Action* _test;
    ACTION _playerAction;

    void AddBlackLadybug();                 // �����Ă�Ƃ����ݒu
    void ActionConvey();

    cocos2d::TMXTiledMap* mapData;          // ����ڲ԰
};

#endif // __HELLOWORLD_SCENE_H__
