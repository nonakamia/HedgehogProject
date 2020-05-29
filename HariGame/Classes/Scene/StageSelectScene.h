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

	void changeScene(Ref* pSender, std::string map);				// ｽﾃｰｼﾞｾﾚｸﾄ画面に移動

private:
	CREATE_FUNC(StageSelectScene);
	
	void SetMenu(Ref* pSender);										// ﾒﾆｭｰ画面を表示する
};

