#ifndef WALL_H_
#define WALL_H_

#include "GameObject.h"

class Wall : public GameObject
{
private:

public:
	Wall(const Point & position);
};

#endif /* WALL_H_ */