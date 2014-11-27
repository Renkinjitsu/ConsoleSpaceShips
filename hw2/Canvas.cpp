#include "Canvas.h"

#include "io_utils.h"

Canvas::Canvas()
{
}

void Canvas::setCharacter(unsigned x, unsigned y, char character)
{
	gotoxy((int)x, (int)y);
	std::cout << character;
}

void Canvas::setCharacter(unsigned x, unsigned y, int num_character)
{
	gotoxy((int)x, (int)y);
	std::cout << num_character;
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
	return Canvas::width;
}

unsigned Canvas::getHeight()
{
	return Canvas::height;
}
