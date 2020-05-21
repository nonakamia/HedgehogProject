#pragma once
#include "cocos2d.h"

#include "Obj/Obj.h"

class Player : public Obj
{
public:
	static Obj* createPlayer(OBJ_COLOR color);
	Player();
	~Player();

	void update(float delta);

	void Rotate();						// 回転
	void Jump();
	void Falling();
	void Change(int color);
	bool CollsionCheck(cocos2d::Vec2 vec);		// 足場との当たり判定

	void DamageAction();						// ﾀﾞﾒｰｼﾞｱｸｼｮﾝ

	void GameOverAction();

	bool SetStartPosition(cocos2d::TMXLayer* startPosLayer, cocos2d::Vec2 tileSize);

	void SetAction(ACTION action);
	ACTION GetAction();

private:
	ACTION _action;								// ｱｸｼｮﾝ

	float _time;
	float _vector;								// 落下速度
	float _maxVec;
	bool _jumpFlag;
	float _airTime;								// 滞空時間

	std::map<ACTION, std::list<ACTION>> _blackList;	// ﾌﾞﾗｯｸﾘｽﾄ

	
	cocos2d::Action* _rollingAction;			// 回転ｱｸｼｮﾝ

	cocos2d::Action* _damageAction;				// ﾀﾞﾒｰｼﾞｱｸｼｮﾝ

	cocos2d::Action* _gemeOverAction;			// ｹﾞｰﾑｵｰﾊﾞｰｱｸｼｮﾝ

	void Jumping();
	void Rolling(float delta);
};

