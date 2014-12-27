#ifndef BIG_SHIP_H_
#define BIG_SHIP_H_

#include "Ship.h"

class BigShip : public Ship
{
public:
	BigShip(const Point & bottoLeftPosition);

	bool isBlockingRotation(const GameObject & other) const;
	void rotate();
};

#endif /* BIG_SHIP_H_ */