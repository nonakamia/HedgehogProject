#include "Obstacles.h"

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
