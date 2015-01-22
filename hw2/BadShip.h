#ifndef BAD_SHIP_H_
#define BAD_SHIP_H_

#include "Ship.h"

class BadShip : public Ship
{
public:
	BadShip(const Point & bottoLeftPosition);

	bool isBlockingRotation(const GameObject & other) const;
	void rotate();
};

#endif /* BAD_SHIP_H_ */