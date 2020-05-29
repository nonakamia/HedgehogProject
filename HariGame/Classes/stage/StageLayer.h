#pragma once
#include "cocos2d.h"

class StageLayer : public cocos2d::Layer
{
public:
	static Layer* createStageLayer(std::string map, cocos2d::Vec2 pos);
	StageLayer(std::string map, cocos2d::Vec2 pos);
	~StageLayer();
	bool init();
private:
	//CREATE_FUNC(StageLayer);

	std::string _map;
	cocos2d::Vec2 _position;

	cocos2d::Sprite* _callout;
	bool _calloutFlag;
};