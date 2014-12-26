#include "GameConfig.h"
#include "Canvas.h"
#include "Keyboard.h"
#include "ScreenManager.h"
#include "MainMenuScreen.h"

int main(int argc, char * argv[])
{
	if(argc > 1)
	{
		GameConfig::setLevelsPath(argv[1]);
	}

	Canvas canvas;
	Keyboard keyboard;

	ScreenManager::add(new MainMenuScreen());
	ScreenManager::run(canvas, keyboard);

	return 0;
}