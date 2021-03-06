#pragma once
#include "Obj/Obstacles/Obstacles.h"
#include "cocos2d.h"

class CheckPoint : public Obstacles
{
public:
	static Obj* createCheckPoint();
	CheckPoint();
	~CheckPoint();
	bool init();

	void DamageAction(cocos2d::Sprite* spite);						// ﾀﾞﾒｰｼﾞｱｸｼｮﾝ
	void GameOverAction();
	void GameClearAction();

	void HitCheck(cocos2d::Node* players, HPMng* playerHP);
private:
	CkSound* _checkPointSE;		// 通過した時のSE
};

