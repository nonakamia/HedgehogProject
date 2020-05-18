#pragma once
#include "Obj/Obj.h"
#include "cocos2d.h"

class BlackLadybug : public Obj
{
public:
	static Obj* createBlackLadybug(OBJ_COLOR color);
	BlackLadybug();
	~BlackLadybug();
	void HitAction();
private:
	bool _hitFlag;
};

