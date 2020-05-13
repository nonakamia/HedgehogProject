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

	void PlayerRolling();						// ��]
	bool CollsionCheck(cocos2d::Vec2 vec);		// ���ꓖ���蔻��
private:
	ACTION _action;
};

