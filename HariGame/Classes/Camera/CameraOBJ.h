#pragma once
#include "cocos2d.h"
#include "Scene/GameScene.h"

struct CameraOBJ
{
	bool operator()(cocos2d::CameraFlag flag,cocos2d::Scene* scene);
};