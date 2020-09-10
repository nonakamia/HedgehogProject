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
	bool init();

	void update(float delta);

	void DamageAction(cocos2d::Sprite* spite);						// ��Ұ�ޱ����
	void GameOverAction();
	void GameClearAction();

	void HitCheck(cocos2d::Node* players, HPMng* playerHP);
private:
	bool _passingFlag;												// �ʉ߂�����

	//@cricket
	CkSound* _openSE;												// �J�Ԏ���SE
};

