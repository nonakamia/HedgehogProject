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
		MENU,
		MAX
	};

	BaseScene();
	~BaseScene();

	void SetMenuFlag(bool flag);

private:

protected:
	bool _changeSceneFlag;
	bool _menuFlag;

};
