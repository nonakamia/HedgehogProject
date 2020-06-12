#include "Obstacles.h"

Obstacles::Obstacles()
{
	_obstaclesBank = nullptr;
}

Obstacles::~Obstacles()
{
	if (_obstaclesBank)
	{
		_obstaclesBank->destroy();
		_obstaclesBank = nullptr;
	}
}

bool Obstacles::init()
{
	if (!Sprite::init())
	{
		return false;
	}

	//@cricket
#ifdef CK_PLATFORM_WIN
	_obstaclesBank = CkBank::newBank("Resources/se/obstacles/obstacles.ckb");
#else
	_obstaclesBank = CkBank::newBank("se/obstacles/obstacles.ckb");
#endif
	return true;
}

bool Obstacles::Check(Obj* player)
{

	if ((player->getPosition().x + player->GetPoint().x <= this->getPosition().x + this->GetPoint().x) &&
		(player->getPosition().x + player->GetPoint().x >= this->getPosition().x - this->GetPoint().x) &&
		(player->getPosition().y - player->GetPoint().y <= this->getPosition().y + this->GetPoint().y) &&
		(player->getPosition().y - player->GetPoint().y >= this->getPosition().y - this->GetPoint().y))
	{
		return true;
	}

	return false;
}
