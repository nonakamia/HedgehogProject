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

	bool changeScene(Ref* pSender,std::string stageName, std::string map);				// ½Ã°¼Þ¾Ú¸Ä‰æ–Ê‚ÉˆÚ“®

private:
	CREATE_FUNC(StageSelectScene);
	std::vector<StageData> _stage;		// ½Ã°¼ÞÃÞ°À

	cocos2d::MenuItemImage* _button;								// ÒÆ­°ÎÞÀÝ

	cocos2d::Vec2 touchPos;

	//@cricket
	CkSound* _selectSound;
	
	void AddScrollAction();											// ½Ã°¼Þ‚Ì½¸Û°Ù
	void SetMenu(Ref* pSender);										// ÒÆ­°‰æ–Ê‚ð•\Ž¦‚·‚é
};

