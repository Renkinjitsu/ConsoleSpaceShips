#ifndef INSTRUCTION_SCREEN_H_
#define INSTRUCTION_SCREEN_H_

#include "Screen.h"

class InstructionsScreen : public Screen
{
private:
	static const char * const _instructions[];
	static const size_t _instructionsCount;

	unsigned _startX;
	unsigned _startY;

public:
	InstructionsScreen();
	~InstructionsScreen();

	void setInitialState();
	void readUserInput(const Keyboard & keyboard);
	void process();
	void update();
	void draw(Canvas & canvas) const;
};

#endif /* INSTRUCTION_SCREEN_H_ */
