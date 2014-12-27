#include "BigShip.h"

BigShip::BigShip(const Point & bottoLeftPosition) : Ship(bottoLeftPosition, 2, 2, '#')
{
	//Nothing to do
}

bool BigShip::isBlockingRotation(const GameObject & other) const
{
	const std::vector<Point> & points = this->getPoints();

	const unsigned left = Point::getLeft(Point::getLeft(points[0], points[1]), Point::getLeft(points[2], points[3]));
	const unsigned top = Point::getTop(Point::getTop(points[0], points[1]), Point::getTop(points[2], points[3]));
	Point point(left, top);
	point.move(Point::UP);

	GameObject blockingArea(' ', false);
	blockingArea.setPoints(point);

	point.move(Point::RIGHT);
	blockingArea.setPoints(point);

	return other.collidesWith(blockingArea);
}

void BigShip::rotate()
{
	//Nothing todo
}
