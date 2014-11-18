#include "Wall.h"

Wall::Wall(unsigned x, unsigned y) : GameObject(x, y)
{
}

void Wall::draw(Canvas & canvas)
{
	canvas.setCharacter(this->getX(), this->getY(), '+');
}
