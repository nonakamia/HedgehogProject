#include "ActionConvey.h"

USING_NS_CC;

ActionConvey* ActionConvey::createActionConvey()
{
	return ActionConvey::create();
}

ActionConvey::ActionConvey()
{
	_conveyAction = nullptr;
}

ActionConvey::~ActionConvey()
{
}

bool ActionConvey::init()
{
	auto players = Director::getInstance()->getRunningScene()->getChildByName("PLAYER_LAYER");

	_player_front = (Player*)players->getChildByName("player_front");
	_player_behind = (Player*)players->getChildByName("player_behind");

	return true;
}

void ActionConvey::update(float delta)
{
	if (_conveyAction == nullptr)
	{
		return;
	}

	if (_conveyAction->isDone())
	{
		CC_SAFE_RELEASE_NULL(_conveyAction);

		_player_behind->SetAction(_actionList.front());
		_actionList.erase(_actionList.begin());
		//for (auto action : _actionList)
		//{
		//	action
		//}
		
		return;
	}

}

void ActionConvey::SetActionConvey(ACTION action)
{
	_actionList.push_back(action);

	_conveyAction = runAction(DelayTime::create(0.6f));
	CC_SAFE_RETAIN(_conveyAction);
}
