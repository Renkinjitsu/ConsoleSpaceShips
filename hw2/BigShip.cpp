#include "BigShip.h"

#include "GameConfig.h"

BigShip::BigShip(const Point & bottoLeftPosition) : Ship(bottoLeftPosition, 2, 2, GameConfig::TEXTURES_BIG_SPACESHIP)
{
	//Nothing to do
}

bool BigShip::isBlockingRotation(const GameObject & other) const
{
	return false;
}

void BigShip::rotate()
{
	//Nothing todo
}
