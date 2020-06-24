#pragma once
#include "cocos2d.h"

//@cricket
#include "ck/ck.h"
#include "ck/config.h"
#include "ck/bank.h"
#include "ck/sound.h"

struct StageData
{
	std::string stageName;		// ½Ã°¼Ş–¼
	std::string imagePass;		// ½Ã°¼Ş–¼(‰æ‘œ)
	std::string mapData;		// Ï¯ÌßÃŞ°À
};

class StageLayer : public cocos2d::Layer
{
public:
	static Layer* createStageLayer(StageData data);
	StageLayer(StageData data);
	~StageLayer();
	bool init();

	void update(float delta);

	void SetMinimumLayerPosX(float posX);
	float GetMinimumLayerPosX();

private:
	//CREATE_FUNC(StageLayer);

	float _minimumLayerPosX;

	bool _selectFlag;

	StageData _stageData;				// ½Ã°¼Şî•ñ
	int _rnk;							// ‰ß‹‚ÌÅ‘å•]‰¿(0`3)
	cocos2d::Vec2 _position;

	cocos2d::Sprite* _callout;
	bool _calloutFlag;

	//@cricket
	CkBank* _buttonBank;				// ÎŞÀİBank
	CkSound* _buttonSE;					// ÎŞÀİ‰¹
};