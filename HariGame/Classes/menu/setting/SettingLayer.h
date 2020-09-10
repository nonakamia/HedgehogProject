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
	std::string _ifs;

	cocos2d::Action* _backAction;

	// ç≈ëÂHP
	cocos2d::Label* _hpLabel;
	int _maxHP;
	int _defaultHP;

	void BackMenu(Ref* pSender);
	void HP(Ref* pSender, int addHP);
	void Reset(Ref* pSender);
};

