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

	void changeScene(Ref* pSender);									// ｽﾃｰｼﾞｾﾚｸﾄ画面に移動
	void SetMenu(Ref* pSender);										// ﾒﾆｭｰ画面を表示する
};

