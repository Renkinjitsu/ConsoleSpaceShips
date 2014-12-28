#ifndef IN_GAME_MENU_SCREEN_H_
#define IN_GAME_MENU_SCREEN_H_

#include "MenuScreen.h"

class InGameMenuScreen : public MenuScreen
{
private:
	//None

public:
	InGameMenuScreen();
	~InGameMenuScreen();

	void setInitialState();
	void readUserInput(const Keyboard & keyboard);
	void process();
	void update();
};

#endif /* IN_GAME_MENU_SCREEN_H_ */
