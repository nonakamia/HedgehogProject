#pragma once
#include "cocos2d.h"

#include "Action/ActionConvey.h"

class ButtonLayer : public cocos2d::Layer
{
public:
	static Layer* createButtonLayer();
	ButtonLayer();
	~ButtonLayer();
	bool init();
private:
	CREATE_FUNC(ButtonLayer);
};