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

	void DamageAction(cocos2d::Sprite* spite) override;						// ﾀﾞﾒｰｼﾞｱｸｼｮﾝ
	void GameOverAction() override;
	void GameClearAction() override;

	void HitCheck(cocos2d::Node* players,HPMng* playerHP) override;
private:
	CkSound* _hitSE;												// 当たった時のSE
};

