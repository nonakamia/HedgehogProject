#pragma once
#include "Obj/Obstacles/Obstacles.h"
#include "cocos2d.h"
#include "HP/HPMng.h"

class Bomb : public Obstacles
{
public:
	static Obj* createBomb(OBJ_COLOR color);
	Bomb();
	~Bomb();
	bool init();

	void update(float delta) override;

	void DamageAction(cocos2d::Sprite* spite) override;						// ÀÞÒ°¼Þ±¸¼®Ý
	void GameOverAction() override;
	void GameClearAction() override;

	void HitCheck(cocos2d::Node* players, HPMng* playerHP) override;
private:
	CkSound* _hitSE;												// “–‚½‚Á‚½Žž‚ÌSE
};

