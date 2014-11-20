#include "Ship.h"

Ship::Ship(unsigned xPositin, unsigned yPosition,
	unsigned width, unsigned height,
	char texture) : GameObject(xPositin, yPosition), texture(texture)
{
	this->width = width;
	this->height = height;
}

void Ship::draw(Canvas & canvas)
{
	for (unsigned i = 0; i < this->width; i++)
	{
		for (unsigned j = 0; j < this->height; j++)
		{
			unsigned x = this->getXstart() + this->width - i;
			unsigned y = this->getYstart() + this->height - j;

			canvas.setCharacter(x, y, this->texture);
		}
	}
}
