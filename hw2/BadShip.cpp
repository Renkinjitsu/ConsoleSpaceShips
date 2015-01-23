#include "BadShip.h"

#include "GameConfig.h"

BadShip::BadShip(const Point & bottomLeftPosition) : Ship(bottomLeftPosition, 1, 1, GameConfig::TEXTURES_BAD_SPACESHIP)
{
	//Nothing to do
}

bool BadShip::isBlockingRotation(const GameObject & other) const
{
	return false;
}

void BadShip::rotate()
{
	//Nothing to do
}
