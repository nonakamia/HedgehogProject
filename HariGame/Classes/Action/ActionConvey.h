#pragma once
#include "cocos2d.h"
#include "ACTION.h"

struct NotesCheck
{
	void operator()(ACTION action);		// 前のﾌﾟﾚｲﾔｰが行ったｱｸｼｮﾝを1.5秒後に後ろのﾌﾟﾚｲﾔｰが行う
};

