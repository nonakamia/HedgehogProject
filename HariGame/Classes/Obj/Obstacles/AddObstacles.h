#pragma once
#include "cocos2d.h"

// 障害物
enum OBSTACLES
{
    LADYBUG_R = 315,    // 黒いてんとう虫(赤)
    LADYBUG_G = 316,    // 黒いてんとう虫(緑)
    BUDS_R = 320,       // ﾂﾎﾞﾐ(赤)
    BUDS_G = 321,       // ﾂﾎﾞﾐ(緑)
    BOM_R = 322,        // 爆弾(赤)
    BOM_G = 323,        // 爆弾(緑)
    LAMP = 311,         // ﾗﾝﾌﾟ(ｺﾞｰﾙ)
    C_POINT = 312,      // ﾁｪｯｸﾎﾟｲﾝﾄ
    MAX
};

struct AddObstacles
{
	void operator()(cocos2d::Layer* layer, cocos2d::TMXLayer* obstaclesLayer);
};

