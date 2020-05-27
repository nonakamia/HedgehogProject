#pragma once
#include "cocos2d.h"

class BaseScene : public cocos2d::Scene
{
public:
	// zOlder(���s��)
	enum class zOlder {
		BG,			// �w�i
		OBSTACLES,	// ��Q��
		PLAYER,		// ��ڲ԰
		BUTTON,		// ����
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
