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
		MAX
	};

	BaseScene();
	~BaseScene();

private:

protected:
	bool _changeSceneFlag;

};
