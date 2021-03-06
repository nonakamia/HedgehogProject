#pragma once
#include "cocos2d.h"
#include "BaseScene.h"
#include "stage/StageLayer.h"

class StageSelectScene : public BaseScene
{
public:
	static cocos2d::Scene* createStageSelectScene();
	StageSelectScene();
	~StageSelectScene();
	bool init();

	void update(float delta);

	void Resume();

	bool changeScene(Ref* pSender,std::string stageName, std::string map);				// ｽﾃｰｼﾞｾﾚｸﾄ画面に移動

private:
	CREATE_FUNC(StageSelectScene);
	std::vector<StageData> _stage;		// ｽﾃｰｼﾞﾃﾞｰﾀ

	cocos2d::MenuItemImage* _button;								// ﾒﾆｭｰﾎﾞﾀﾝ

	cocos2d::Vec2 touchPos;

	//@cricket
	CkSound* _selectSound;
	
	void AddScrollAction();											// ｽﾃｰｼﾞのｽｸﾛｰﾙ
	void SetMenu(Ref* pSender);										// ﾒﾆｭｰ画面を表示する
};

