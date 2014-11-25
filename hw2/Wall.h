#ifndef WALL_H_
#define WALL_H_

#include "GameObject.h"

class Wall : public GameObject
{
private:

public:
	Wall(unsigned x, unsigned y);

	void draw(Canvas & canvas);

	void EraseDrawing(Canvas & canvas);
	};

#endif /* WALL_H_ */