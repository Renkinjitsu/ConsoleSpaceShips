#include "Canvas.h"

#include "io_utils.h"

#include <assert.h>

unsigned Canvas::calculateIndexFromPosition(unsigned x, unsigned y)
{
	unsigned index = ((GameConfig::SCREEN_HEIGHT - y - 1) * GameConfig::SCREEN_WIDTH) + x;

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

void Canvas::draw(unsigned x, unsigned y, const std::string & string)
{
	this->draw(x, y, string.c_str());
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
	return GameConfig::SCREEN_WIDTH;
}

unsigned Canvas::getHeight()
{
	return GameConfig::SCREEN_HEIGHT;
}
