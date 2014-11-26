#include "Wall.h"

Wall::Wall(unsigned x, unsigned y) : GameObject(OBJECT_WALL, x, y)
{
}

void Wall::EraseDrawing(Canvas & canvas)
{
	vector<Object_location_t> & locations = get_locations();
	for (vector<Object_location_t>::iterator it = locations.begin(); it != locations.end(); ++it)
	{
		canvas.setCharacter(it->x, it->y, ' ');
	}
}

void Wall::draw(Canvas & canvas)
{
	vector<Object_location_t> location = this->get_locations();

	for (vector<Object_location_t>::iterator it = location.begin(); it != location.end(); ++it)
	{
		canvas.setCharacter(it->x, it->y, '+');
	}
}
