#pragma once

// プレイヤーのアクション
enum class ACTION {
	NON,
	ROLLING,	//回転
	MAX
};

ACTION begin(ACTION);
ACTION end(ACTION);
ACTION operator++(ACTION& action);
ACTION operator* (ACTION& action);