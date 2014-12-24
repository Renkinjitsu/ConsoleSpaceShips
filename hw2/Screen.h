#ifndef SCREEN_H_
#define SCREEN_H_

#include "Canvas.h"
#include "Keyboard.h"

class Screen
{
private:
	//None

public:
	Screen();
	~Screen();

	virtual void setInitialState() = 0;
	virtual void readUserInput(const Keyboard & keyboard) = 0;
	virtual void process() = 0;
	virtual void update() = 0; //The actual moving/translation/advancment of the game
	virtual void draw(Canvas & canvas) const = 0;
};

#endif /* SCREEN_H_ */
