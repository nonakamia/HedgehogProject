#pragma once
#include "cocos2d.h"

class SettingLayer : public cocos2d::Layer
{
public:
	static Layer* createSettingLayer();
	SettingLayer();
	~SettingLayer();
	bool init();
	void update(float delta);
private:
	CREATE_FUNC(SettingLayer);

	cocos2d::Action* _backAction;

	void BackMenu(Ref* pSender);
};

