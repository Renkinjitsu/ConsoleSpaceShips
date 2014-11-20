#include "Wall.h"

Wall::Wall(unsigned x, unsigned y) : GameObject(x, y)
{
}

void Wall::draw(Canvas & canvas)
{
	vector<Item_location_t> location = this->get_locations();

	//Item_location_t first_loc = location.pop

	for (vector<Item_location_t>::iterator it = location.begin(); it != location.end(); ++it) 
	{
		canvas.setCharacter(it->x, it->y, '+');
	}
}
