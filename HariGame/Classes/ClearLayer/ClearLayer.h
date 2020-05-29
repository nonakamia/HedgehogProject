#pragma once

#include "cocos2d.h"

class ClearLayer:public cocos2d::Layer
{
public:
	static Layer* createClearLayer();
	ClearLayer();
	~ClearLayer();
	bool init();

private:
	CREATE_FUNC(ClearLayer);

	bool _visible;

	void BackStageSelectScene(Ref* pSender);
};