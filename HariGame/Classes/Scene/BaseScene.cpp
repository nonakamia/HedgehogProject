#include "BaseScene.h"

USING_NS_CC;

BaseScene::BaseScene()
{
	// ����
	_changeSceneFlag = false;
	_menuFlag = false;
}

BaseScene::~BaseScene()
{
}

bool BaseScene::GetMenuFlag()
{
	return _menuFlag;
}
