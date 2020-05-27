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

	void changeScene(Ref* pSender);					// ½Ã°¼Ş¾Ú¸Ä‰æ–Ê‚Ö‚ÌØ‘Ö—pŠÖ”

private:
	CREATE_FUNC(TitleScene);
	
	cocos2d::Point _touchPoint;

	cocos2d::Sprite* _endGameImage;
	bool _endGameFlag;								// ƒQ[ƒ€I—¹‰æ–Ê
	cocos2d::MenuItemImage* _yesButton;
	cocos2d::MenuItemImage* _noButton;

	void SetEndGame(Ref* pSender);
	void EndGame(Ref* pSender);
};

