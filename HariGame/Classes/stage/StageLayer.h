#pragma once
#include "cocos2d.h"

//@cricket
#include "ck/ck.h"
#include "ck/config.h"
#include "ck/bank.h"
#include "ck/sound.h"

class StageLayer : public cocos2d::Layer
{
public:
	static Layer* createStageLayer(std::string map, cocos2d::Vec2 pos);
	StageLayer(std::string map, cocos2d::Vec2 pos);
	~StageLayer();
	bool init();

	void update(float delta);
private:
	//CREATE_FUNC(StageLayer);

	bool _selectFlag;

	std::string _map;
	cocos2d::Vec2 _position;

	cocos2d::Sprite* _callout;
	bool _calloutFlag;

	//@cricket
	CkBank* _buttonBank;							// ÎÞÀÝBank
	CkSound* _buttonSE;								// ÎÞÀÝ‰¹
};