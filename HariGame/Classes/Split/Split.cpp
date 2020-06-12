#include "Split.h"

USING_NS_CC;

cocos2d::ValueVector Split::split(const std::string& str, const std::string& delim)
{
	ValueVector res;
	size_t current = 0, found;
	while ((found = str.find_first_of(delim, current)) != std::string::npos) {
		res.push_back(Value(std::string(str, current, found - current)));
		current = found + 1;
	}
	res.push_back(Value(std::string(str, current, str.size() - current)));
	return res;
}
