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
	void Resume();                          // �ްэĊJ

private:
	CREATE_FUNC(TitleScene);
	
	cocos2d::Point _touchPoint;

	cocos2d::Sprite* _endGameImage;
	bool _endGameFlag;								// �Q�[���I�����
	cocos2d::MenuItemImage* _yesButton;
	cocos2d::MenuItemImage* _noButton;

	void changeScene(Ref* pSender);					// �ð�޾ڸĉ�ʂւ̐ؑ֗p�֐�

	void SetEndGame(Ref* pSender);
	void EndGame(Ref* pSender);

	
};

