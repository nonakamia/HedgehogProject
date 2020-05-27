#include "BaseScene.h"

USING_NS_CC;

BaseScene::BaseScene()
{
	// ‹¤’Ê
	_changeSceneFlag = false;
	_menuFlag = false;
}

BaseScene::~BaseScene()
{
}

void BaseScene::SetMenuFlag(bool flag)
{
	_menuFlag = flag;
}
