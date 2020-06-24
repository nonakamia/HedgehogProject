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

	bool changeScene(Ref* pSender,std::string stageName, std::string map);				// �ð�޾ڸĉ�ʂɈړ�

private:
	CREATE_FUNC(StageSelectScene);
	std::vector<StageData> _stage;		// �ð���ް�

	cocos2d::MenuItemImage* _button;								// �ƭ�����

	cocos2d::Vec2 touchPos;

	//@cricket
	CkSound* _selectSound;
	
	void AddScrollAction();											// �ð�ނ̽�۰�
	void SetMenu(Ref* pSender);										// �ƭ���ʂ�\������
};

