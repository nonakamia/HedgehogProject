#pragma once
#include "cocos2d.h"
#include "BaseScene.h"


class TitleScene : public BaseScene
{
public:
	static cocos2d::Scene* createTitleScene();
	TitleScene();
	~TitleScene();
	bool init();

	void changeScene(Ref* pSender);					// ｽﾃｰｼﾞｾﾚｸﾄ画面への切替用関数

private:
	CREATE_FUNC(TitleScene);
};

