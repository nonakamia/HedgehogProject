#pragma once
#include "cocos2d.h"

// áŠQ•¨
enum OBSTACLES
{
    LADYBUG_R = 315,    // •‚¢‚Ä‚ñ‚Æ‚¤’(Ô)
    LADYBUG_G = 316,    // •‚¢‚Ä‚ñ‚Æ‚¤’(—Î)
    BUDS_R = 320,       // ÂÎŞĞ(Ô)
    BUDS_G = 321,       // ÂÎŞĞ(—Î)
    LAMP = 311,         // ×İÌß(ºŞ°Ù)
    MAX
};

struct AddObstacles
{
	void operator()(cocos2d::Layer* layer, cocos2d::TMXLayer* obstaclesLayer);
};

