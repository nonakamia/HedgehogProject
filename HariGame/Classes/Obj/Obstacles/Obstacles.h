#pragma once
#include "Obj/Obj.h"

class Obstacles:public Obj
{
public:
	virtual void HitCheck(cocos2d::Node* players, HPMng* playerHP) = 0;
protected:
	bool Check(Obj* player);
};

