#pragma once
#include "cocos2d.h"

// ��Q��
enum OBSTACLES
{
    LADYBUG_R = 216,    // �����Ă�Ƃ���(��)
    LADYBUG_G = 217,    // �����Ă�Ƃ���(��)
    BUDS_R = 224,       // ����(��)
    BUDS_G = 225,       // ����(��)
    LAMP = 219,         // ����(�ް�)
    MAX
};

struct AddObstacles
{
	void operator()(cocos2d::Layer* layer, cocos2d::TMXLayer* obstaclesLayer);
};

