#include "SmallShip.h"

SmallShip::SmallShip(unsigned xPositin, unsigned yPosition) : Ship(xPositin, yPosition, 2, 1, '@')
{
}

void SmallShip::rotate()
{
	this->width ^= this->height;
	this->height ^= this->width;
	this->width ^= this->height;
}
