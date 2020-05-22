#pragma once
#include "Obj/Obstacles/Obstacles.h"
#include "cocos2d.h"

#include "HP/HPMng.h"

class BlackLadybug : public Obstacles
{
public:
	static Obj* createBlackLadybug(OBJ_COLOR color);
	BlackLadybug();
	~BlackLadybug();
	void DamageAction();						// ÀÞÒ°¼Þ±¸¼®Ý
	void GameOverAction();
	void HitCheck(cocos2d::Node* players,HPMng* playerHP);
private:
};

