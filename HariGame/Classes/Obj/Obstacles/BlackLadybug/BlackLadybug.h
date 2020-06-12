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
	bool init();

	void DamageAction(cocos2d::Sprite* spite);						// ﾀﾞﾒｰｼﾞｱｸｼｮﾝ
	void GameOverAction();
	void GameClearAction();

	void HitCheck(cocos2d::Node* players,HPMng* playerHP);
private:
	CkSound* _hitSE;												// 当たった時のSE
};

