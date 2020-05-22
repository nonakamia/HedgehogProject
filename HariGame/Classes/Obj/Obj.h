#pragma once
#include "cocos2d.h"

#include "Action/ACTION.h"
#include "Obj/OBJ_COLLAR.h"
#include "HP/HPMng.h"

class Obj : public cocos2d::Sprite
{
public:
	virtual void DamageAction() = 0;						// ÀÞÒ°¼Þ±¸¼®Ý
	virtual void GameOverAction() = 0;

	virtual void HitCheck(cocos2d::Node* players, HPMng* playerHP) = 0;

	void SetPoint(cocos2d::Vec2 point);
	cocos2d::Vec2 GetPoint();

	bool GetDamageFlag();
protected:
	cocos2d::Vec2 _point;						// “–‚½‚è”»’è”ÍˆÍ
	bool _damageFlag;
	bool _gameOverFlag;
};

