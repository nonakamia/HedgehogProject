#pragma once
#include "cocos2d.h"

#include "obj/Obj.h"
#include "Action/ACTION.h"

class Player : public Obj
{
public:
	static Obj* createPlayer();
	Player();
	~Player();

	void PlayerRolling();

private:
	ACTION action;
};

