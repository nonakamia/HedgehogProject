#pragma once
#include "cocos2d.h"
#include "GameScene.h"

struct CameraOBJ
{
	bool operator()(cocos2d::CameraFlag flag,cocos2d::Scene* scene);
};