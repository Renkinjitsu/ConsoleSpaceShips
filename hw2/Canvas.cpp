#include "Canvas.h"

#include "io_utils.h"

#include <assert.h>

unsigned Canvas::calculateIndexFromPosition(unsigned x, unsigned y)
{
	unsigned index = ((Canvas::height - y - 1) * Canvas::width) + x;

	assert(index < 1920);

	return index;
}

Canvas::Canvas()
{
	//Nothing to do
}

void Canvas::draw(unsigned x, unsigned y, char character)
{
	this->_buffer[Canvas::calculateIndexFromPosition(x, y)] = character;
}

void Canvas::draw(unsigned x, unsigned y, const char * string)
{
	unsigned i = Canvas::calculateIndexFromPosition(x, y);

	while (*string != '\0')
	{
		this->_buffer[i++] = *string;
		++string;
	}
}

void Canvas::save()
{
	for(unsigned i = 0; i < Canvas::bufferLength; i++)
	{
		this->_bufferBackup[i] = this->_buffer[i];
	}

	this->_bufferBackup[Canvas::bufferLength] = '\0';
}

void Canvas::restore()
{
	for(unsigned i = 0; i < Canvas::bufferLength; i++)
	{
		this->_buffer[i] = this->_bufferBackup[i];
	}

	this->_buffer[Canvas::bufferLength] = '\0';
}

void Canvas::begin()
{
	for(unsigned i = 0; i < Canvas::bufferLength; i++)
	{
		this->_buffer[i] = ' ';
	}

	this->_buffer[Canvas::bufferLength] = '\0';
}

void Canvas::end()
{
	gotoxy(0, 0);
	std::cout << this->_buffer;
}

unsigned Canvas::getWidth()
{
	return Canvas::width;
}

unsigned Canvas::getHeight()
{
	return Canvas::height;
}
