#ifndef WALL_H_
#define WALL_H_

#include "GameObject.h"

class Wall : GameObject
{
private:

public:
	Wall(unsigned x, unsigned y);

	void draw(Canvas & canvas);
	};

#endif /* WALL_H_ */