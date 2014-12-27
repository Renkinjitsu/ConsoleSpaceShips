#include "Canvas.h"

#include "io_utils.h"

#include <assert.h>

const Point Canvas::TOP_LEFT(0, 0);
const Point Canvas::BOTTOM_LEFT(0, GameConfig::SCREEN_HEIGHT - 1);
const Point Canvas::TOP_RIGHT(GameConfig::SCREEN_WIDTH - 1, 0);
const Point Canvas::BOTTOM_RIGHT(GameConfig::SCREEN_WIDTH - 1, GameConfig::SCREEN_HEIGHT - 1);
const Point Canvas::CENTER(GameConfig::SCREEN_WIDTH / 2, GameConfig::SCREEN_HEIGHT / 2);

unsigned Canvas::getWidth()
{
	return GameConfig::SCREEN_WIDTH;
}

unsigned Canvas::getHeight()
{
	return GameConfig::SCREEN_HEIGHT;
}

unsigned Canvas::serialize(const Point & posititon)
{
	const unsigned x = posititon.getX();
	const unsigned y = posititon.getY();

	const unsigned index = (y * GameConfig::SCREEN_WIDTH) + x;

	assert(index < Canvas::bufferLength);

	return index;
}

Point Canvas::deserialize(const unsigned serializedPosititon)
{
	assert(serializedPosititon < Canvas::bufferLength);

	const unsigned x = serializedPosititon % Canvas::getWidth();
	const unsigned y = serializedPosititon / Canvas::getWidth();

	return Point(x, y);
}

Canvas::Canvas()
{
	//Nothing to do
}

void Canvas::draw(const Point & posititon, char character)
{

	this->_buffer[Canvas::serialize(posititon)] = character;
}

void Canvas::draw(const Point & posititon, const char * string)
{
	unsigned i = Canvas::serialize(posititon);

	while(*string != '\0')
	{
		this->_buffer[i++] = *string;
		++string;
	}
}

void Canvas::draw(const Point & posititon, const std::string & string)
{
	this->draw(posititon, string.c_str());
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
