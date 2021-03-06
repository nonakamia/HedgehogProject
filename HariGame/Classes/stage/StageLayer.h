#pragma once
#include "cocos2d.h"

//@cricket
#include "ck/ck.h"
#include "ck/config.h"
#include "ck/bank.h"
#include "ck/sound.h"

struct StageData
{
	std::string stageName;		// ｽﾃｰｼﾞ名
	std::string imagePass;		// ｽﾃｰｼﾞ名(画像)
	std::string mapData;		// ﾏｯﾌﾟﾃﾞｰﾀ
};

class StageLayer : public cocos2d::Layer
{
public:
	static Layer* createStageLayer(StageData data);
	StageLayer(StageData data);
	~StageLayer();
	bool init();

	void update(float delta);

	// 初期のﾚｲﾔｰの位置
	void SetMinimumLayerPosX(float posX);
	float GetMinimumLayerPosX();

	// 表示ﾌﾗｸﾞ
	bool GetCalloutFlag();

private:
	//CREATE_FUNC(StageLayer);

	float _minimumLayerPosX;

	bool _selectFlag;

	StageData _stageData;				// ｽﾃｰｼﾞ情報
	int _rank;							// 過去の最大評価(0〜3)
	cocos2d::Vec2 _position;

	cocos2d::Sprite* _callout;
	bool _calloutFlag;

	//@cricket
	CkBank* _buttonBank;				// ﾎﾞﾀﾝBank
	CkSound* _buttonSE;					// ﾎﾞﾀﾝ音
};