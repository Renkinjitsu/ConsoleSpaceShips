#include "Point.h"

#include "Canvas.h"

Point::Point(unsigned x, unsigned y)
{
	this->_x = x;
	this->_y = y;
}

unsigned Point::getX() const
{
	return this->_x;
}

unsigned Point::getY() const
{
	return this->_y;
}

bool Point::equals(unsigned x, unsigned y) const
{
	return (this->_x == x && this->_y == y);
}

bool Point::equals(const Point & other) const
{
	return this->equals(other._x, other._y);
}

void Point::move(Direction direction)
{
	switch(direction)
	{
		case DIRECTION_UP:
		{
			this->_y = (this->_y + 1) % Canvas::getHeight();
		}
		break;

		case DIRECTION_DOWN:
		{
			this->_y = (this->_y + Canvas::getHeight() - 1) % Canvas::getHeight();
		}
		break;

		case DIRECTION_LEFT:
		{
			this->_x = (this->_x + Canvas::getWidth() - 1) % Canvas::getWidth();
		}
		break;

		case DIRECTION_RIGHT:
		{
			this->_x = (this->_x + 1) % Canvas::getWidth();
		}
		break;
	}
}
