#pragma once
#include "Obj/Obstacles/Obstacles.h"
#include "cocos2d.h"

#include "HP/HPMng.h"


class Buds : public Obstacles
{
public:
	static Obj* createBuds(OBJ_COLOR color);
	Buds();
	~Buds();

	void update(float delta);

	void DamageAction();						// ��Ұ�ޱ����
	void GameOverAction();
	void HitCheck(cocos2d::Node* players, HPMng* playerHP);
private:
	bool _openFlag;								// �J�Ԃ��Ă��邩
};
