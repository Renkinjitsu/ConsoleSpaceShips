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

bool Ship::isPresent()
{
	return this->_isPresent;
}

void Ship::explode()
{
	this->_isAlive = false;
}

void Ship::disappear()
{
	this->_isPresent = false;
}
