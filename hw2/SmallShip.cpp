#include "SmallShip.h"

#include "GameConfig.h"
#include "Area.h"

SmallShip::SmallShip(const Point & bottoLeftPosition, bool horizontal)
	: Ship(bottoLeftPosition, horizontal ? 2 : 1, horizontal ? 1 : 2, GameConfig::TEXTURES_SMALL_SPACESHIP)
{
	//Do nothing
}

bool SmallShip::isBlockingRotation(const GameObject & other) const
{
	const std::vector<Point> & points = this->getPoints();

	const unsigned left = Point::getLeft(points[0], points[1]);
	const unsigned bottom = Point::getBottom(points[0], points[1]);

	const Point topLeft = Point(left, bottom) + Point::UP;
	const Area blockingArea(topLeft, 2, 2);

	return other.collidesWith(blockingArea);
}

void SmallShip::rotate()
{
	std::vector<Point> points = this->getPoints();
	this->clearPoints();

	const unsigned left = Point::getLeft(points[0], points[1]);
	const unsigned bottom = Point::getBottom(points[0], points[1]);
	const unsigned right = Point::getRight(points[0], points[1]);
	const unsigned top = Point::getTop(points[0], points[1]);

	Point bottomLeft(left, bottom);
	Point topRight(right, top);

	if(bottomLeft.getX() == topRight.getX()) //Vertical
	{
		topRight += Point::RIGHT;
		topRight += Point::DOWN;
	}
	else //Horizontal
	{
		topRight += Point::UP;
		topRight += Point::LEFT;
	}

	this->setPoints(bottomLeft);
	this->setPoints(topRight);
};
