#pragma once
#include "cocos2d.h"

class BaseScene : public cocos2d::Scene
{
public:
	// zOlder(‰œs‚«)
	enum class zOlder {
		BG,			// ”wŒi
		OBSTACLES,	// áŠQ•¨
		PLAYER,		// ÌßÚ²Ô°
		BUTTON,		// ÎŞÀİ
		HP,
		MAX
	};

	BaseScene();
	~BaseScene();

private:

protected:
	bool _changeSceneFlag;

};
