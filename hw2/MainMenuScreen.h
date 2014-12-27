#ifndef MAIN_MENU_SCREEN_H_
#define MAIN_MENU_SCREEN_H_

#include <vector>

#include "MenuScreen.h"

class MainMenuScreen : public MenuScreen
{
private:
	//None

public:
	MainMenuScreen();
	~MainMenuScreen();

	void setInitialState();
	void readUserInput(const Keyboard & keyboard);
	void process();
	void update();
};

#endif /* MAIN_MENU_SCREEN_H_ */
