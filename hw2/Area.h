#ifndef AREA
#define AREA

#include "GameObject.h"

class Area : public GameObject
{
private:

public:
	Area(const Point & topLeft, unsigned width, unsigned height);
};

#endif /* Area */
