#include "Obj.h"

void Obj::SetPoint(cocos2d::Vec2 point)
{
	_point = point;
}

cocos2d::Vec2 Obj::GetPoint()
{
	return _point;;
}

bool Obj::GetDamageFlag()
{
	return _damageFlag;
}
