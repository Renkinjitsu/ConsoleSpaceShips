#include "SmallShip.h"

SmallShip::SmallShip(unsigned xPositin, unsigned yPosition) : Ship(xPositin, yPosition, 2, 1, '@')
{
	//Do nothing
}

bool SmallShip::isBlockingRotation(const GameObject & other)
{
	std::vector<Point> locations = this->getLocations();

	unsigned x = locations[0].getX();
	unsigned y = locations[0].getY();

	GameObject rotationArea(' ', false);
	rotationArea.setLocations(x + 0, y + 0);
	rotationArea.setLocations(x + 1, y + 0);
	rotationArea.setLocations(x + 0, y + 1);
	rotationArea.setLocations(x + 1, y + 1);

	return rotationArea.collidesWith(other);
}

void SmallShip::rotate()
{
	std::vector<Point> locations = this->getLocations();
	this->clearLocations();

	Point bottomLeft = locations[0];
	Point other = locations[1];

	if(bottomLeft.getX() == other.getX())
	{
		other.move(DIRECTION_RIGHT);
		other.move(DIRECTION_DOWN);
	}
	else
	{
		other.move(DIRECTION_UP);
		other.move(DIRECTION_LEFT);
	}

	this->setLocations(bottomLeft);
	this->setLocations(other);
};
