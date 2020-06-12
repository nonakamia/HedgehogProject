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

	void Resume();

private:
	CREATE_FUNC(MenuLayer);
	enum MENU_SELECT
	{
		MENU_CANCEL,
		SROM_SCRATCH,
		BACK_TITLE,
		BACK_SELECT,
		MAX
	};

	cocos2d::MenuItemImage* _cancelButton;

	bool _menuFlag;

	//@cricket
	CkBank* _buttonBank;							// ÎŞÀİBank
	CkSound* _buttonSE;								// ÎŞÀİ‰¹

	void MenuCancel(Ref* pSender);
	void SromScratch(Ref* pSender);
	void BackTitleScene(Ref* pSender);			// À²ÄÙ‰æ–Ê‚É–ß‚é
	void BackStageSelectScene(Ref* pSender);	// ½Ã°¼Ş¾Ú¸Ä‰æ–Ê‚É–ß‚é
	void AddSettingLayer(Ref* pSender);			// İ’è‰æ–Ê‚ğ•\¦
};