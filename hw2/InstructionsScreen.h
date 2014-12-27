#ifndef INSTRUCTION_SCREEN_H_
#define INSTRUCTION_SCREEN_H_

#include "MenuScreen.h"

class InstructionsScreen : public MenuScreen
{
private:
	//None

public:
	InstructionsScreen();
	~InstructionsScreen();

	void setInitialState();
	void readUserInput(const Keyboard & keyboard);
	void process();
	void update();
};

#endif /* INSTRUCTION_SCREEN_H_ */
