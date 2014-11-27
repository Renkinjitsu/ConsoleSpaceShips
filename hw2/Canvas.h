#ifndef CANVAS_H_
#define CANVAS_H_

class Canvas
{
private:
	static const unsigned width = 80;
	static const unsigned height = 24;

public:
	static unsigned getWidth();
	static unsigned getHeight();

	Canvas();

	void setCharacter(unsigned x, unsigned y, char character);
	void setCharacter(unsigned x, unsigned y, int num_character);
	void clear();
};

#endif /* CANVAS_H_ */