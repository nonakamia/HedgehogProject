#pragma once

// プレイヤーのアクション
enum class ACTION {
	NON,
	ROLL,		// 回転
	ROLLING,	// 回転中
	FALL,		// 落下
	JUMP,		// ｼﾞｬﾝﾌﾟ
	JUMPING,	// ｼﾞｬﾝﾌﾟ中
	DAMAGE,		// ﾀﾞﾒｰｼﾞ
	MAX
};

ACTION begin(ACTION);
ACTION end(ACTION);
ACTION operator++(ACTION& action);
ACTION operator* (ACTION& action);