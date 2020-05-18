#include "BlackLadybug.h"

USING_NS_CC;

Obj* BlackLadybug::createBlackLadybug(OBJ_COLOR color)
{
	Obj* pRet = new(std::nothrow) BlackLadybug();
	if (pRet && pRet->init())
	{
		if (color == OBJ_COLOR::OBJ_RED)
		{
			pRet->setSpriteFrame(Sprite::create("obstacles/blackLadybug_r.png")->getSpriteFrame());
		}
		else if(color == OBJ_COLOR::OBJ_GREEN)
		{
			pRet->setSpriteFrame(Sprite::create("obstacles/blackLadybug_g.png")->getSpriteFrame());
		}

		pRet->setTag(static_cast<int>(color));
		pRet->setAnchorPoint(Point(0.5f, 0.5f));
		return pRet;
	}
	else
	{
		delete pRet;
		pRet = nullptr;
		return nullptr;
	}
}

BlackLadybug::BlackLadybug()
{
	_hitFlag = false;
}

BlackLadybug::~BlackLadybug()
{
}

void BlackLadybug::HitAction()
{
	if (!_hitFlag)
	{
		setVisible(false);
		_hitFlag = true;
	}
}
