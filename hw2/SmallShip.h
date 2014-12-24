#ifndef SMALL_SHIP_H_
#define SMALL_SHIP_H_

#include "Ship.h"

class SmallShip : public Ship
{

public:
	SmallShip(unsigned xPositin, unsigned yPosition, bool horizontal);

	bool isBlockingRotation(const GameObject & other);
	void rotate();
};

#endif /* SMALL_SHIP_H_ */