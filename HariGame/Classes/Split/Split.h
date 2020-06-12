#pragma once
#include "cocos2d.h"

struct Split
{
	static cocos2d::ValueVector split(const std::string& str, const std::string& delim);
};

