#pragma once
#include "cocos2d.h"

#include "Obj/Obj.h"
#include "Action/ACTION.h"
#include "Obj/OBJ_COLLAR.h"

class Player : public Obj
{
public:
	static Obj* createPlayer(OBJ_COLOR playerColor,cocos2d::Vec2 point);
	Player(cocos2d::Vec2 point);
	~Player();

	void update(float delta);

	void PlayerRolling();						// 回転
	void Jump();
	void Falling();
	void Change(int color);
	bool CollsionCheck(cocos2d::Vec2 vec);		// 足場との当たり判定

	void setAction(ACTION action);
private:
	cocos2d::Vec2 _point;						// 当たり判定範囲
	ACTION _action;								// ｱｸｼｮﾝ

	float _time;
	float _fallV;								// 落下速度
};

