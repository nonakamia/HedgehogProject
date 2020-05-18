#pragma once
#include "cocos2d.h"

#include "Obj/Obj.h"

class Player : public Obj
{
public:
	static Obj* createPlayer(OBJ_COLOR color,cocos2d::Vec2 point);
	Player(cocos2d::Vec2 point);
	~Player();

	void update(float delta);

	void PlayerRolling();						// 回転
	void Jump();
	void Falling();
	void Change(int color);
	bool CollsionCheck(cocos2d::Vec2 vec);		// 足場との当たり判定

	void DamageAction();						// ﾀﾞﾒｰｼﾞｱｸｼｮﾝ

	void SetAction(ACTION action);
	cocos2d::Vec2 getPoint();
private:
	cocos2d::Vec2 _point;						// 当たり判定範囲
	ACTION _action;								// ｱｸｼｮﾝ

	float _time;
	float _vector;								// 落下速度
	float _maxVec;
	bool _jumpFlag;
	float _airTime;								// 滞空時間

	std::map<ACTION, std::list<ACTION>> _blackList;	// ﾌﾞﾗｯｸﾘｽﾄ

	cocos2d::Action* _rollingAction;			// 回転ｱｸｼｮﾝ
	cocos2d::Action* _damageAction;
	bool damageFlag;

	void Jumping();

};

