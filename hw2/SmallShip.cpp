#include "SmallShip.h"

#include "GameConfig.h"

SmallShip::SmallShip(const Point & bottoLeftPosition, bool horizontal)
	: Ship(bottoLeftPosition, horizontal ? 2 : 1, horizontal ? 1 : 2, GameConfig::TEXTURES_SMALL_SPACESHIP)
{
	//Do nothing
}

bool SmallShip::isBlockingRotation(const GameObject & other) const
{
	const std::vector<Point> & points = this->getPoints();

	const unsigned left = Point::getLeft(points[0], points[1]);
	const unsigned top = Point::getTop(points[0], points[1]);
	Point point(left, top);

	GameObject blockingArea(GameConfig::TEXTURES_EMPTY, false);

	point.move(Point::UP);
	blockingArea.setPoints(point);

	if(points[0].getX() == points[1].getX()) //Vertical
	{
		point.move(Point::RIGHT);
		point.move(Point::DOWN);
		blockingArea.setPoints(point);

		point.move(Point::DOWN);
		blockingArea.setPoints(point);
	}
	else //Horizontal
	{
		point.move(Point::RIGHT);
		blockingArea.setPoints(point);
	}

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
		topRight.move(Point::RIGHT);
		topRight.move(Point::DOWN);
	}
	else //Horizontal
	{
		topRight.move(Point::UP);
		topRight.move(Point::LEFT);
	}

	this->setPoints(bottomLeft);
	this->setPoints(topRight);
};
