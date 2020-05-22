#pragma once
#include "Obj/Obj.h"

#include "cocos2d.h"

class Buds : public Obj
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

