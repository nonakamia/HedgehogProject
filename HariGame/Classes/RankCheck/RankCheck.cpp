#include "RankCheck.h"

USING_NS_CC;

void RankCheck::rankCheck(std::string name, int hp)
{
	// UserDefault呼び出し
	UserDefault* _userDef = UserDefault::getInstance();

	int maxHP = _userDef->getIntegerForKey("MAX_HP");

	// ﾀﾞﾒｰｼﾞをくらった数
	int distance = maxHP - hp;
	// ﾗﾝｸ = ﾀﾞﾒｰｼﾞ数(0ﾀﾞﾒｰｼﾞ(ﾉｰﾀﾞﾒｰｼﾞ):3, 1ﾀﾞﾒｰｼﾞ:2, 2ﾀﾞﾒｰｼﾞ:1, 3以上ﾀﾞﾒｰｼﾞ:0)
	if (3 > distance)
	{
		int rank = 3 - distance;
		// 今回のｽﾃｰｼﾞでのﾗﾝｸを記録する
		_userDef->setIntegerForKey("nowRank", rank);

		// 以前のﾗﾝｸより上なら更新する
		if (rank > _userDef->getIntegerForKey(name.c_str()))
		{
			_userDef->setIntegerForKey(name.c_str(), rank);
		}
	}
	else
	{
		_userDef->setIntegerForKey("nowRank", 0);
	}

	
}
