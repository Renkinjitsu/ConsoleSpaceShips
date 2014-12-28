#ifndef SCREEN_MANAGER_H_
#define SCREEN_MANAGER_H_

#include <vector>

#include "Screen.h"

typedef std::vector<Screen *> Screens;

class ScreenManager
{
private:
	static const unsigned TICKS_PER_SECOND = 8;

	static Screens _screens;
	static Screen * _removeScreen;
	static bool _removeAll;

	ScreenManager();
	~ScreenManager();
public:

	static void add(Screen * screen);
	static void remove(Screen * screen);
	static void removeAll();

	static void run(Canvas & canvas, Keyboard & keyboard);
};

#endif /* SCREEN_MANAGER_H_ */
