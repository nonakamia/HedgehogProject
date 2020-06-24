#pragma once
#include "cocos2d.h"

//@cricket
#include "ck/ck.h"
#include "ck/config.h"
#include "ck/bank.h"
#include "ck/sound.h"

struct StageData
{
	std::string stageName;		// �ð�ޖ�
	std::string imagePass;		// �ð�ޖ�(�摜)
	std::string mapData;		// ϯ���ް�
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

	StageData _stageData;				// �ð�ޏ��
	int _rnk;							// �ߋ��̍ő�]��(0�`3)
	cocos2d::Vec2 _position;

	cocos2d::Sprite* _callout;
	bool _calloutFlag;

	//@cricket
	CkBank* _buttonBank;				// ����Bank
	CkSound* _buttonSE;					// ���݉�
};