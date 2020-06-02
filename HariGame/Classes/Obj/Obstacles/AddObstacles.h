#pragma once
#include "cocos2d.h"

// áŠQ•¨
enum OBSTACLES
{
    LADYBUG_R = 216,    // •‚¢‚Ä‚ñ‚Æ‚¤’(Ô)
    LADYBUG_G = 217,    // •‚¢‚Ä‚ñ‚Æ‚¤’(—Î)
    BUDS_R = 224,       // ÂÎŞĞ(Ô)
    BUDS_G = 225,       // ÂÎŞĞ(—Î)
    LAMP = 219,         // ×İÌß(ºŞ°Ù)
    MAX
};

struct AddObstacles
{
	void operator()(cocos2d::Layer* layer, cocos2d::TMXLayer* obstaclesLayer);
};

