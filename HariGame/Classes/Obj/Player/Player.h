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

	void PlayerRolling();						// ‰ñ“]
	bool CollsionCheck(cocos2d::Vec2 vec);		// ‘«ê“–‚½‚è”»’è
private:
	ACTION _action;
};

