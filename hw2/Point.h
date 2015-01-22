#ifndef POINT_H_
#define POINT_H_

class Point
{
private:
	unsigned _x;
	unsigned _y;

	bool equals(unsigned x, unsigned y) const;

	void move(unsigned x, unsigned y);

public:
	static const Point UP;
	static const Point DOWN;
	static const Point LEFT;
	static const Point RIGHT;

	static const Point ZERO;

	static const Point DIRECTIONS[];
	static const unsigned DIRECTIONS_COUNT;


	static unsigned getLeft(unsigned x1, unsigned x2);
	static unsigned getBottom(unsigned y1, unsigned y2);
	static unsigned getRight(unsigned x1, unsigned x2);
	static unsigned getTop(unsigned y1, unsigned y2);

	static unsigned getLeft(const Point & p1, const Point & p2);
	static unsigned getBottom(const Point & p1, const Point & p2);
	static unsigned getRight(const Point & p1, const Point & p2);
	static unsigned getTop(const Point & p1, const Point & p2);

	Point(unsigned x = 0, unsigned y = 0);
	Point(const Point & other);

	unsigned getX() const;
	unsigned getY() const;

	Point & operator+=(const Point & other);
	Point & operator*=(const unsigned scalar);
	Point & operator/=(const unsigned scalar);

	Point operator+(const Point & other) const;
	Point operator*(const unsigned scalar) const;
	Point operator/(const unsigned scalar) const;

	bool operator==(const Point & other) const;
	bool operator!=(const Point & other) const;
};

#endif /* POINT_H_ */
