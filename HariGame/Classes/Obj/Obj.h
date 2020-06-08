#pragma once
#include "cocos2d.h"

#include "Action/ACTION.h"
#include "Obj/OBJ_COLLAR.h"
#include "HP/HPMng.h"

class Obj : public cocos2d::Sprite
{
public:
	virtual void DamageAction(cocos2d::Sprite* spite) = 0;						// ��Ұ�ޱ����
	virtual void GameOverAction() = 0;
	virtual void GameClearAction() = 0;

	void SetPoint(cocos2d::Vec2 point);
	cocos2d::Vec2 GetPoint();

	bool GetDamageFlag();
protected:
	cocos2d::Vec2 _point;						// �����蔻��͈�
	bool _damageFlag;
	bool _gameOverFlag;
};

