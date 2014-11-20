#ifndef CANVAS_H_
#define CANVAS_H_

class Canvas
{
private:
	unsigned width;
	unsigned height;

public:
	Canvas(unsigned width, unsigned height);

	void setCharacter(unsigned x, unsigned y, char character);
	void setCharacter(unsigned x, unsigned y, int num_character);
	void clear();

	unsigned getWidth();
	unsigned getHeight();
};

#endif /* CANVAS_H_ */