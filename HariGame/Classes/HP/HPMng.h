#pragma once
#include "cocos2d.h"

class HPMng : public cocos2d::Layer
{
public:
	static HPMng* createHPMng(int maxHP);
	HPMng(int maxHP);
	~HPMng();
	bool init(); 
private:
	//CREATE_FUNC(HPMng);

	int _maxHP;
};

