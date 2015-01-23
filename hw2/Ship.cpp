#include "Ship.h"

Ship::Ship(const Point & bottomLeftPosition, unsigned width, unsigned height, char texture)
	: GameObject(texture, false)
{
	Point rowWalker(bottomLeftPosition);
	for(size_t i = 0; i < height; ++i)
	{
		Point point(rowWalker);
		for(size_t j = 0; j < width; ++j)
		{
			this->setPoints(point);

			point += Point::RIGHT;
		}

		rowWalker += Point::UP;
	}
}

void Ship::explode()
{
	//Do nothing
}
