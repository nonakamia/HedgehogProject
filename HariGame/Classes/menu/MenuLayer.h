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
	void BackTitleScene(Ref* pSender);	// ���ى�ʂɖ߂�
	void BackStageSelectScene(Ref* pSender);	// �ð�޾ڸĉ�ʂɖ߂�
};