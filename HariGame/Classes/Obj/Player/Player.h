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

	void PlayerRolling();						// 回転
	bool CollsionCheck(cocos2d::Vec2 vec);		// 足場当たり判定
private:
	ACTION _action;
};

