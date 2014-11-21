#include "Ship.h"

Ship::Ship(unsigned xPositin, unsigned yPosition,
	unsigned width, unsigned height,
	char texture) : GameObject(xPositin, yPosition), texture(texture)
{
	this->width = width;
	this->height = height;

	vector<Object_location_t> locations;

	for (size_t i = 0; i < width; i++)
	{
		for (size_t j = 0; j < height; j++)
		{
			Object_location_t location;
			location.x = xPositin + i;
			location.y = yPosition + j;

			locations.push_back(location);
		}
	}

	set_locations(locations);
}

void Ship::draw(Canvas & canvas)
{
	vector<Object_location_t> locations = get_locations();
	for (vector<Object_location_t>::iterator it = locations.begin(); it != locations.end(); ++it)
	{
		canvas.setCharacter(it->x, it->y, this->texture);
	}
}
