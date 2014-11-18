#ifndef SHIP_H_
#define SHIP_H_

#include "GameObject.h"

class Ship : GameObject
{
private:
	const char texture;

protected:
	unsigned width;
	unsigned height;

public:
	Ship(unsigned xPositin, unsigned yPosition,
		unsigned width, unsigned height,
		char texture);

	void draw(Canvas & canvas);
};

#endif /* SHIP_H_ */