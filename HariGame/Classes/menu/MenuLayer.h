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
	void BackTitleScene(Ref* pSender);	// ﾀｲﾄﾙ画面に戻る
	void BackStageSelectScene(Ref* pSender);	// ｽﾃｰｼﾞｾﾚｸﾄ画面に戻る
};