#ifndef SHIP_H_
#define SHIP_H_

#include "GameObject.h"

class Ship : public GameObject
{
private:
	//None

public:
	Ship(const Point & bottomLeftPosition, unsigned width, unsigned height, char texture);

	virtual bool isBlockingRotation(const GameObject & other) const = 0;

	void explode();
	virtual void rotate() = 0;
};

#endif /* SHIP_H_ */