#include "Ship.h"

Ship::Ship(const Point & bottoLeftPosition, unsigned width, unsigned height, char texture)
	: GameObject(texture, false)
{
	Point startingPoint(bottoLeftPosition);
	for(size_t i = 0; i < width; i++)
	{
		startingPoint.move(Point::RIGHT);

		Point current(startingPoint);
		for(size_t j = 0; j < height; j++)
		{
			current.move(Point::UP);
			this->setPoints(current);
		}
	}
}

void Ship::explode()
{
	this->setTexture((char)13);
}
