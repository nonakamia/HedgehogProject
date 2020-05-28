#pragma once
#include "cocos2d.h"
#include "BaseScene.h"

class StageSelectScene : public BaseScene
{
public:
	static cocos2d::Scene* createStageSelectScene();
	StageSelectScene();
	~StageSelectScene();
	bool init();

	void Resume();

private:
	CREATE_FUNC(StageSelectScene);

	cocos2d::Sprite* _callout;
	bool _calloutFlag;

	void changeScene(Ref* pSender);									// ½Ã°¼Þ¾Ú¸Ä‰æ–Ê‚ÉˆÚ“®
	void SetMenu(Ref* pSender);										// ÒÆ­°‰æ–Ê‚ð•\Ž¦‚·‚é
};

