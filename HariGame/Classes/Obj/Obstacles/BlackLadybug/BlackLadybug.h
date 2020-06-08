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
	void DamageAction(cocos2d::Sprite* spite);						// ÀÞÒ°¼Þ±¸¼®Ý
	void GameOverAction();
	void GameClearAction();

	void HitCheck(cocos2d::Node* players,HPMng* playerHP);
private:
};

