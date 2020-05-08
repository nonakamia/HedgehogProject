#pragma once
#include "cocos2d.h"

#include "obj/Obj.h"

class Player : public Obj
{
public:
	static Obj* createPlayer();
	Player();
	~Player();
};

