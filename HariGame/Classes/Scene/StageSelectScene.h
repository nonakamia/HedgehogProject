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

	void changeScene(Ref* pSender, std::string map);				// �ð�޾ڸĉ�ʂɈړ�

private:
	CREATE_FUNC(StageSelectScene);
	
	void SetMenu(Ref* pSender);										// �ƭ���ʂ�\������
};

