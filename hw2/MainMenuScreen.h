#ifndef MAIN_MENU_SCREEN_H_
#define MAIN_MENU_SCREEN_H_

#include <vector>

#include "Screen.h"

class MainMenuScreen : public Screen
{
private:
	std::vector<const char *> _menuItems;

	unsigned _startX;
	unsigned _startY;

public:
	MainMenuScreen();
	~MainMenuScreen();

	void setInitialState();
	void readUserInput(const Keyboard & keyboard);
	void process();
	void update();
	void draw(Canvas & canvas) const;
};

#endif /* MAIN_MENU_SCREEN_H_ */
