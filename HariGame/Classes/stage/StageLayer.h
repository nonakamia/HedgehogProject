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
	static Layer* createStageLayer(std::string name, std::string map);
	StageLayer(std::string name, std::string map);
	~StageLayer();
	bool init();

	void update(float delta);

	void SetMinimumLayerPosX(float posX);
	float GetMinimumLayerPosX();

private:
	//CREATE_FUNC(StageLayer);

	float _minimumLayerPosX;

	bool _selectFlag;

	std::string _name;
	std::string _map;
	cocos2d::Vec2 _position;

	cocos2d::Sprite* _callout;
	bool _calloutFlag;

	cocos2d::Label* _stageLabel;

	//@cricket
	CkBank* _buttonBank;							// ÎÞÀÝBank
	CkSound* _buttonSE;								// ÎÞÀÝ‰¹
};