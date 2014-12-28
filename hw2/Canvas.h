#ifndef CANVAS_H_
#define CANVAS_H_

#include <ostream>

#include "GameConfig.h"
#include "Point.h"

class Canvas
{
private:
	static const unsigned bufferLength = (GameConfig::SCREEN_WIDTH * GameConfig::SCREEN_HEIGHT);

	char _buffer[bufferLength + 1];
	char _bufferBackup[bufferLength + 1];

public:
	static const unsigned MAX_SERIALIZED_LENGTH = bufferLength;

	static unsigned getWidth();
	static unsigned getHeight();

	static unsigned serialize(const Point & posititon);
	static Point deserialize(const unsigned serializedPosititon);

	static const Point TOP_LEFT;
	static const Point BOTTOM_LEFT;
	static const Point TOP_RIGHT;
	static const Point BOTTOM_RIGHT;
	static const Point CENTER;

	Canvas();
	~Canvas();

	void draw(const Point & posititon, char character);
	void draw(const Point & posititon, const char * string);
	void draw(const Point & posititon, const std::string & string);

	void save();
	void restore();

	void begin();
	void end();
	void end(std::string & destination);
};

#endif /* CANVAS_H_ */