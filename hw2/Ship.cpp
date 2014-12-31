#include "Ship.h"

Ship::Ship(const Point & bottoLeftPosition, unsigned width, unsigned height, char texture)
	: GameObject(texture, false)
{
	Point widthWalker(bottoLeftPosition);
	for(size_t i = 0; i < width; i++)
	{
		Point heightWalker(widthWalker);
		for(size_t j = 0; j < height; j++)
		{
			this->setPoints(heightWalker);
			heightWalker.move(Point::UP);
		}

		widthWalker.move(Point::RIGHT);
	}
}

void Ship::explode()
{
	//Do nothing
}
