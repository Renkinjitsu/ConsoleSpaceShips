#ifndef EXIT_POINT_H_
#define EXIT_POINT_H_

#include "GameObject.h"

class ExitPoint : public GameObject
{
private:

public:
	ExitPoint(const std::vector<Point> & points);
};

#endif