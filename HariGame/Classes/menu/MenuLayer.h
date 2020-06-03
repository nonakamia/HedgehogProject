#pragma once
#include "cocos2d.h"

//@cricket
#include "ck/ck.h"
#include "ck/config.h"
#include "ck/bank.h"
#include "ck/sound.h"

class MenuLayer : public cocos2d::Layer
{
public:
	static Layer* createMenuLayer();
	MenuLayer();
	~MenuLayer();
	bool init();

	void update(float delta);

private:
	CREATE_FUNC(MenuLayer);

	bool _menuFlag;

	//@cricket
	CkBank* _buttonBank;							// ÎÞÀÝBank
	CkSound* _buttonSE;								// ÎÞÀÝ‰¹

	void MenuCancel(Ref* pSender);
	void SromScratch(Ref* pSender);
	void BackTitleScene(Ref* pSender);	// À²ÄÙ‰æ–Ê‚É–ß‚é
	void BackStageSelectScene(Ref* pSender);	// ½Ã°¼Þ¾Ú¸Ä‰æ–Ê‚É–ß‚é
};