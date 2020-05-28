#pragma once
#include "cocos2d.h"
#include "ACTION.h"
#include "Obj/Player/Player.h"

#include <vector>

struct ActionConvey : public cocos2d::Node
{
public:
	static ActionConvey* createActionConvey();

	ActionConvey();
	~ActionConvey();
	bool init();

	void update(float delta);

	void SetActionConvey(ACTION action);		// 前のﾌﾟﾚｲﾔｰが行ったｱｸｼｮﾝを0.6秒後に後ろのﾌﾟﾚｲﾔｰが行う
	void ConveyClear();
private:
	CREATE_FUNC(ActionConvey);

	Player* _player_behind;

	std::vector<ACTION> _actionList;

	std::vector <cocos2d::Action*> _conveyAction;
};

