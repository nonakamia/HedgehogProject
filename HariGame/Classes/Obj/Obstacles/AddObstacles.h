#pragma once
#include "cocos2d.h"

// ��Q��
enum OBSTACLES
{
    LADYBUG_R = 315,    // �����Ă�Ƃ���(��)
    LADYBUG_G = 316,    // �����Ă�Ƃ���(��)
    BUDS_R = 320,       // ����(��)
    BUDS_G = 321,       // ����(��)
    LAMP = 311,         // ����(�ް�)
    MAX
};

struct AddObstacles
{
	void operator()(cocos2d::Layer* layer, cocos2d::TMXLayer* obstaclesLayer);
};

