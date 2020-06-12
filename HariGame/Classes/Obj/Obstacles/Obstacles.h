#pragma once
#include "Obj/Obj.h"

//@cricket
#include "ck/ck.h"
#include "ck/config.h"
#include "ck/bank.h"
#include "ck/sound.h"

class Obstacles:public Obj
{
public:
	Obstacles();
	virtual ~Obstacles();
	bool init();
	virtual void HitCheck(cocos2d::Node* players, HPMng* playerHP) = 0;
protected:
	CkBank* _obstaclesBank;

	bool Check(Obj* player);
};

