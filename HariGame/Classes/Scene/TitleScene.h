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

	void update(float delta);

	void Resume();								   // ｹﾞｰﾑ再開

private:
	CREATE_FUNC(TitleScene);
	
	cocos2d::Point _touchPoint;

	cocos2d::Sprite* _endGameImage;
	bool _endGameFlag;								// ゲーム終了画面
	cocos2d::MenuItemImage* _yesButton;
	cocos2d::MenuItemImage* _noButton;

	//@cricket
	CkBank* _buttonBank;							// ﾎﾞﾀﾝBank
	CkSound* _buttonSE;								// ﾎﾞﾀﾝ音
	CkSound* _titleSound;

	void SetChangeScene(Ref* pSender);
	void ChangeScene();					// ｽﾃｰｼﾞｾﾚｸﾄ画面への切替用関数

	void SetEndGame(Ref* pSender);
	void EndGame(Ref* pSender);

	
};

