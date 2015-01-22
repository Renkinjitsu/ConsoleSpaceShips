#include "Point.h"

#include "GameConfig.h"

#include <algorithm>

const Point Point::UP(0, GameConfig::SCREEN_HEIGHT - 1);
const Point Point::DOWN(0, 1);
const Point Point::LEFT(GameConfig::SCREEN_WIDTH - 1, 0);
const Point Point::RIGHT(1, 0);

const Point Point::ZERO(0, 0);

const Point Point::DIRECTIONS[] = {Point::UP, Point::DOWN, Point::LEFT, Point::RIGHT};
const unsigned Point::DIRECTIONS_COUNT = sizeof(Point::DIRECTIONS) / sizeof(Point::DIRECTIONS[0]);

unsigned Point::getLeft(unsigned x1, unsigned x2)
{
	return std::min(x1, x2);
}

unsigned Point::getBottom(unsigned y1, unsigned y2)
{
	return std::max(y1, y2); //Y axis is inverted (to lower value is the higher height)
}

unsigned Point::getRight(unsigned x1, unsigned x2)
{
	return std::max(x1, x2);
}

unsigned Point::getTop(unsigned y1, unsigned y2)
{
	return std::min(y1, y2); //Y axis is inverted (to lower value is the higher height)
}

unsigned Point::getLeft(const Point & p1, const Point & p2)
{
	return Point::getLeft(p1.getX(), p2.getX());
}

unsigned Point::getBottom(const Point & p1, const Point & p2)
{
	return Point::getBottom(p1.getY(), p2.getY());
}

unsigned Point::getRight(const Point & p1, const Point & p2)
{
	return Point::getRight(p1.getX(), p2.getX());
}

unsigned Point::getTop(const Point & p1, const Point & p2)
{
	return Point::getTop(p1.getY(), p2.getY());
}

Point::Point(unsigned x, unsigned y)
{
	_x = x;
	_y = y;
}

Point::Point(const Point & other)
{
	_x = other._x;
	_y = other._y;
}

unsigned Point::getX() const
{
	return _x;
}

unsigned Point::getY() const
{
	return _y;
}

bool Point::equals(unsigned x, unsigned y) const
{
	return (_x == x && _y == y);
}

void Point::move(unsigned x, unsigned y)
{
	y %= GameConfig::SCREEN_HEIGHT;

	_x = (_x + x) % GameConfig::SCREEN_WIDTH;
	_y = (_y + y) % GameConfig::SCREEN_HEIGHT;
}

Point & Point::operator+=(const Point & other)
{
	this->move(other._x, other._y);

	return *this;
}

Point & Point::operator*=(const unsigned scalar)
{
	_x *= scalar;
	_y *= scalar;

	_x %= GameConfig::SCREEN_WIDTH;
	_y %= GameConfig::SCREEN_HEIGHT;

	return *this;
}

Point & Point::operator/=(const unsigned scalar)
{
	_x /= scalar;
	_y /= scalar;

	return *this;
}

Point Point::operator+(const Point & other) const
{
	return Point(*this) += other;
}

Point Point::operator*(const unsigned scalar) const
{
	return Point(*this) *= scalar;
}

Point Point::operator/(const unsigned scalar) const
{
	return Point(*this) /= scalar;
}

bool Point::operator==(const Point & other) const
{
	return this->equals(other._x, other._y);
}

bool Point::operator!=(const Point & other) const
{
	return !((*this) == other);
}
