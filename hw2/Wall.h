#ifndef WALL_H_
#define WALL_H_

#include "GameObject.h"

class Wall : public GameObject
{
private:

public:
	Wall(const std::vector<Point> & points);
};

#endif /* WALL_H_ */