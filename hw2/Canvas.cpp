#include "Canvas.h"

#include "io_utils.h"

Canvas::Canvas(unsigned width, unsigned height)
{
	this->width = width;
	this->height = height;
}

void Canvas::setCharacter(unsigned x, unsigned y, char character)
{
	gotoxy((int)x, (int)y);
	std::cout << character;
}

void Canvas::clear()
{
	for (unsigned i = 0; i < this->width; i++)
	{
		for (unsigned j = 0; j < this->height; j++)
		{
			this->setCharacter(i, j, ' ');
		}
	}
}

unsigned Canvas::getWidth()
{
	return this->width;
}

unsigned Canvas::getHeight()
{
	return this->height;
}
