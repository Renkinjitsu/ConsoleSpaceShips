#ifndef SHIP_H_
#define SHIP_H_

#include "GameObject.h"

class Ship : public GameObject
{
private:
	bool _isAlive;

public:
	Ship(unsigned xPosition, unsigned yPosition,
		unsigned width, unsigned height,
		char texture);

	bool isAlive();

	void explode();
	void disappear();
};

#endif /* SHIP_H_ */