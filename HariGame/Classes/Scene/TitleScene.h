#pragma once
#include "cocos2d.h"
#include "BaseScene.h"

//@cricket
#include "ck/ck.h"
#include "ck/config.h"
#include "ck/bank.h"
#include "ck/sound.h"


class TitleScene : public BaseScene
{
public:
	static cocos2d::Scene* createTitleScene();
	TitleScene();
	~TitleScene();
	bool init();

	void update(float delta);

	void Resume();                          // �ްэĊJ

private:
	CREATE_FUNC(TitleScene);

	//@cricket
	//CkSound* _testSound;
	
	cocos2d::Point _touchPoint;

	cocos2d::Sprite* _endGameImage;
	bool _endGameFlag;								// �Q�[���I�����
	cocos2d::MenuItemImage* _yesButton;
	cocos2d::MenuItemImage* _noButton;

	void changeScene(Ref* pSender);					// �ð�޾ڸĉ�ʂւ̐ؑ֗p�֐�

	void SetEndGame(Ref* pSender);
	void EndGame(Ref* pSender);

	
};

