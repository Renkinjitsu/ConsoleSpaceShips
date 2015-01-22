#include "Ship.h"

Ship::Ship(const Point & bottoLeftPosition, unsigned width, unsigned height, char texture)
	: GameObject(texture, false)
{
	Point rowWalker(bottoLeftPosition);
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
