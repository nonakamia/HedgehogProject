#pragma once

// �v���C���[�̃A�N�V����
enum class ACTION {
	NON,
	ROLL,		// ��]
	ROLLING,	// ��]��
	FALL,		// ����
	JUMP,		// �ެ���
	JUMPING,	// �ެ��ߒ�
	DAMAGE,		// ��Ұ��
	MAX
};

ACTION begin(ACTION);
ACTION end(ACTION);
ACTION operator++(ACTION& action);
ACTION operator* (ACTION& action);