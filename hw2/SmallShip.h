#ifndef SMALL_SHIP_H_
#define SMALL_SHIP_H_

#include "Ship.h"

class SmallShip : public Ship
{

public:
	SmallShip(const Point & bottoLeftPosition, bool horizontal);

	bool isBlockingRotation(const GameObject & other) const;
	void rotate();
};

#endif /* SMALL_SHIP_H_ */