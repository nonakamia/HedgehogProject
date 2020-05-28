#pragma once
#include "cocos2d.h"

class MenuLayer : public cocos2d::Layer
{
public:
	static Layer* createMenuLayer();
	MenuLayer();
	~MenuLayer();
	bool init();
private:
	CREATE_FUNC(MenuLayer);

	bool _menuFlag;

	void MenuCancel(Ref* pSender);
	void SromScratch(Ref* pSender);
	void BackTitleScene(Ref* pSender);	// À²ÄÙ‰æ–Ê‚É–ß‚é
	void BackStageSelectScene(Ref* pSender);	// ½Ã°¼Þ¾Ú¸Ä‰æ–Ê‚É–ß‚é
};