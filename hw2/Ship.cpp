#include "Ship.h"

Ship::Ship(unsigned xPosition, unsigned yPosition,
	unsigned width, unsigned height,
	char texture) : GameObject(texture, false)
{
	this->_isAlive = true;

	for(size_t i = 0; i < width; i++)
	{
		for(size_t j = 0; j < height; j++)
		{
			this->setLocations(xPosition + i, yPosition + j);
		}
	}
}

bool Ship::isAlive()
{
	return this->_isAlive;
}

void Ship::explode()
{
	this->_isAlive = false;

	//TODO: Implement
}

void Ship::disappear()
{
	this->clearLocations();
}
