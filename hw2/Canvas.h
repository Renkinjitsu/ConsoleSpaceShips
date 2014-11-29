#ifndef CANVAS_H_
#define CANVAS_H_

class Canvas
{
private:
	static const unsigned width = 80;
	static const unsigned height = 24;

	static const unsigned bufferLength = width * height;

	char _buffer[bufferLength + 1];
	char _bufferBackup[bufferLength + 1];

	static unsigned calculateIndexFromPosition(unsigned x, unsigned y);

public:
	static unsigned getWidth();
	static unsigned getHeight();

	Canvas();

	void draw(unsigned x, unsigned y, char character);
	void draw(unsigned x, unsigned y, const char * string);

	void save();
	void restore();

	void begin();
	void end();
};

#endif /* CANVAS_H_ */