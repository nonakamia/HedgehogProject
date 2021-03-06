#pragma once
#include "cocos2d.h"

#include "Obj/Obj.h"

//@cricket
#include "ck/ck.h"
#include "ck/config.h"
#include "ck/bank.h"
#include "ck/sound.h"

class Player : public Obj
{
public:
	static Obj* createPlayer(OBJ_COLOR color);
	Player();
	~Player();
	bool init();

	void update(float delta);

	void Rotate();									// 回転
	void Jump();
	void Falling();
	void Change(int color);
	bool CollsionCheck(cocos2d::Vec2 vec);			// 足場との当たり判定

	void DamageAction(cocos2d::Sprite* spite);		// ﾀﾞﾒｰｼﾞｱｸｼｮﾝ

	void GameOverAction();

	void GameClearAction();

	void Resume();									// 再開

	bool SetStartPosition(cocos2d::TMXLayer* startPosLayer, cocos2d::Vec2 tileSize);

	void SetAction(ACTION action);
	ACTION GetAction();

	void FlowerRolling(bool flag);
	bool GetFlowerFlag();

private:
	OBJ_COLOR _color;
	ACTION _action;									// ｱｸｼｮﾝ

	float _moveVec;									// 1秒当たりの移動距離
	float _time;
	float _vector;									// 落下速度
	float _maxVec;
	bool _jumpFlag;
	float _airTime;									// 滞空時間
	bool _flowerFlag;								// 花の上を歩いているか

	std::map<ACTION, std::list<ACTION>> _blackList;	// ﾌﾞﾗｯｸﾘｽﾄ

	
	cocos2d::Action* _rollingAction;				// 回転ｱｸｼｮﾝ

	cocos2d::Action* _damageAction;					// ﾀﾞﾒｰｼﾞｱｸｼｮﾝ

	cocos2d::Action* _gemeOverAction;				// ｹﾞｰﾑｵｰﾊﾞｰｱｸｼｮﾝ

	std::map<ACTION, std::pair<std::string, std::string>> _image;

	//@cricket
	CkBank* _actionBank;							// ﾎﾞﾀﾝBank
	CkSound* _jumpSE;								// ｼﾞｬﾝﾌﾟ音
	CkSound* _damageSE;								// ﾀﾞﾒｰｼﾞ音
	CkSound* _changeSE;								// 入れ替え音

	void Jumping();
	void Rolling(float delta);
	void Damage();									// ﾀﾞﾒｰｼﾞ状態

	bool SetImage(ACTION action);
};

