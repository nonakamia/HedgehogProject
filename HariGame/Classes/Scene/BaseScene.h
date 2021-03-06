#pragma once
#include "cocos2d.h"

//@cricket
#include "ck/ck.h"
#include "ck/config.h"
#include "ck/bank.h"
#include "ck/sound.h"

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

class BaseScene : public cocos2d::Scene
{
public:
	BaseScene();
	~BaseScene();

	virtual void Resume() = 0;
	
	bool GetMenuFlag();
private:

protected:
	bool _changeSceneFlag;
	bool _menuFlag;

};
