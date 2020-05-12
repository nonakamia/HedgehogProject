#include <type_traits>
#include "ACTION.h"

ACTION begin(ACTION)
{
	return ACTION::NON;
}

ACTION end(ACTION)
{
	return ACTION::MAX;
}

ACTION operator++(ACTION& action)
{
	// Šî’ê‚ÌŒ^‚ð’²‚×‚é‚â‚Â
	return action = ACTION(std::underlying_type< ACTION >::type(action) + 1);
}

ACTION operator*(ACTION& action)
{
	return action;
}