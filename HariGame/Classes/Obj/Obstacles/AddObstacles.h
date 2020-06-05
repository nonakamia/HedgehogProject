#pragma once
#include "cocos2d.h"

// 障害物
enum OBSTACLES
{
    LADYBUG_R = 315,    // 黒いてんとう虫(赤)
    LADYBUG_G = 316,    // 黒いてんとう虫(緑)
    BUDS_R = 320,       // ﾂﾎﾞﾐ(赤)
    BUDS_G = 321,       // ﾂﾎﾞﾐ(緑)
    LAMP = 311,         // ﾗﾝﾌﾟ(ｺﾞｰﾙ)
    MAX
};

struct AddObstacles
{
	void operator()(cocos2d::Layer* layer, cocos2d::TMXLayer* obstaclesLayer);
};

