#ifndef POINT_H_
#define POINT_H_

#include "Direction.h"

class Point
{
private:
	unsigned _x;
	unsigned _y;

public:
	Point(unsigned x, unsigned y);

	unsigned getX() const;
	unsigned getY() const;

	bool equals(unsigned x, unsigned y) const;
	bool equals(const Point & other) const;

	void move(Direction direction);
};

#endif /* POINT_H_ */
