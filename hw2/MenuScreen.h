#ifndef MENU_SCREEN_H_
#define MENU_SCREEN_H_

#include "Screen.h"

#include <string>

class MenuScreen : public Screen
{
private:
	std::vector<std::string> _menuItems;

	unsigned _maxLength;

	Point _drawOffset;

public:
	MenuScreen();
	~MenuScreen();

	unsigned getRowsCount() const;

	void append(const char * const line);
	void append(const std::string & line);

	void setInitialState();
	void readUserInput(const Keyboard & keyboard);
	void process();
	void update();
	void draw(Canvas & canvas) const;
};

#endif /* MENU_SCREEN_H_ */
