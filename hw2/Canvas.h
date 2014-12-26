#ifndef CANVAS_H_
#define CANVAS_H_

#include <string>

#include "GameConfig.h"

class Canvas
{
private:
	static const unsigned bufferLength = GameConfig::SCREEN_WIDTH * GameConfig::SCREEN_HEIGHT;

	char _buffer[bufferLength + 1];
	char _bufferBackup[bufferLength + 1];

	static unsigned calculateIndexFromPosition(unsigned x, unsigned y);

public:
	static unsigned getWidth();
	static unsigned getHeight();

	Canvas();

	void draw(unsigned x, unsigned y, char character);
	void draw(unsigned x, unsigned y, const char * string);
	void draw(unsigned x, unsigned y, const std::string & string);

	void save();
	void restore();

	void begin();
	void end();
};

#endif /* CANVAS_H_ */