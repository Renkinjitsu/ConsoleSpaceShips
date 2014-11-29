#include "Wall.h"

Wall::Wall(unsigned bottomLeftX, unsigned bottomLeftY, unsigned width, unsigned height) : GameObject('+', false)
{
	for(unsigned i = 0; i < width; ++i)
	{
		for(unsigned j = 0; j < height; ++j)
		{
			this->setLocations(bottomLeftX + i, bottomLeftY + j);
		}
	}
}