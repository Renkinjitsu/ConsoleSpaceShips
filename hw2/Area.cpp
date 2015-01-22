#include "Area.h"
#include <vector>

Area::Area(const Point & topLeft, unsigned width, unsigned height) : GameObject(GameConfig::TEXTURES_EMPTY, false)
{
	Point rowWalker = topLeft;

	for(unsigned i = 0; i < height; ++i)
	{
		Point point = rowWalker;

		for(unsigned j = 0; j < width; ++j)
		{
			this->setPoints(point);

			point += Point::RIGHT;
		}

		rowWalker += Point::DOWN;
	}
}
