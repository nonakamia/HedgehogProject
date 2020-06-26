#include "RankCheck.h"

USING_NS_CC;

void RankCheck::rankCheck(std::string name, int hp)
{
	// UserDefault�Ăяo��
	UserDefault* _userDef = UserDefault::getInstance();

	int maxHP = _userDef->getIntegerForKey("MAX_HP");

	// ��Ұ�ނ����������
	int distance = maxHP - hp;
	// �ݸ = ��Ұ�ސ�(0��Ұ��(ɰ��Ұ��):3, 1��Ұ��:2, 2��Ұ��:1, 3�ȏ���Ұ��:0)
	if (3 > distance)
	{
		int rank = 3 - distance;
		// ����̽ð�ނł��ݸ���L�^����
		_userDef->setIntegerForKey("nowRank", rank);

		// �ȑO���ݸ����Ȃ�X�V����
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
