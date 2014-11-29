#ifndef WALL_H_
#define WALL_H_

#include "GameObject.h"

class Wall : public GameObject
{
private:

public:
	Wall(unsigned bottomLeftX, unsigned bottomLeftY, unsigned width, unsigned height);
};

#endif /* WALL_H_ */