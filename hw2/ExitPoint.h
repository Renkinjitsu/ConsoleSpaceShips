#ifndef EXITPOINT_H_
#define EXITPOINT_H_

#include "GameObject.h"

class ExitPoint : public GameObject
{
private:

public:
	ExitPoint(const std::vector<Point> & points);
};

#endif