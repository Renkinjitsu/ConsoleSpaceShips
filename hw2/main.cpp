#include "Canvas.h"
#include "Keyboard.h"
#include "ScreenManager.h"
#include "MainMenuScreen.h"

int main()
{
	Canvas canvas;
	Keyboard keyboard;

	ScreenManager::add(new MainMenuScreen());
	ScreenManager::run(canvas, keyboard);

	return 0;
}