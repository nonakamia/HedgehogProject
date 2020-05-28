#include "ActionConvey.h"

USING_NS_CC;

ActionConvey* ActionConvey::createActionConvey()
{
	return ActionConvey::create();
}

ActionConvey::ActionConvey()
{
	//_conveyAction = nullptr;
}

ActionConvey::~ActionConvey()
{
}

bool ActionConvey::init()
{
	_player_behind = (Player*)Director::getInstance()->getRunningScene()->getChildByName("PLAYER_LAYER")->getChildByName("player_behind");

	return true;
}

void ActionConvey::update(float delta)
{
	if (_conveyAction.begin() == _conveyAction.end())
	{
		return;
	}

	if (_conveyAction.front()->isDone())
	{
		CC_SAFE_RELEASE_NULL(_conveyAction.front());

		_player_behind->SetAction(_actionList.front());
		_actionList.erase(_actionList.begin());
		_conveyAction.erase(_conveyAction.begin());
	}

}

void ActionConvey::SetActionConvey(ACTION action)
{
	_actionList.push_back(action);

	_conveyAction.push_back(runAction(DelayTime::create(0.6f)));
	CC_SAFE_RETAIN(_conveyAction.back());
}

void ActionConvey::ConveyClear()
{
	for (auto action : _conveyAction)
	{
		CC_SAFE_RELEASE_NULL(action);
	}
	_conveyAction.clear();
}
