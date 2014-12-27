#include "ScreenManager.h"

#include <ctime>

#include "io_utils.h" //For Sleep()

Screens ScreenManager::_screens;
Screen * ScreenManager::_removeScreen = NULL;

ScreenManager::ScreenManager()
{
}


ScreenManager::~ScreenManager()
{
}

void ScreenManager::add(Screen * screen)
{
	ScreenManager::_screens.push_back(screen);
}

void ScreenManager::remove(Screen * screen)
{
	ScreenManager::_removeScreen = screen;
}

void ScreenManager::run(Canvas & canvas, Keyboard & keyboard)
{
	const clock_t clocksPerFrame = CLOCKS_PER_SEC / ScreenManager::TICKS_PER_SECOND;

	while(ScreenManager::_screens.empty() == false)
	{
		clock_t startClock = clock();

		const size_t topScreenIndex = ScreenManager::_screens.size() - 1;
		Screen * currentScreen = ScreenManager::_screens[topScreenIndex];

		currentScreen->setInitialState();

		keyboard.update();

		currentScreen->readUserInput(keyboard);
		currentScreen->process();
		currentScreen->update();

		canvas.begin();
		currentScreen->draw(canvas);
		canvas.end();

		if(ScreenManager::_removeScreen)
		{
			Screens::iterator iter = std::find(ScreenManager::_screens.begin(),
				ScreenManager::_screens.end(), ScreenManager::_removeScreen);

			if(ScreenManager::_screens.end() != iter)
			{
				ScreenManager::_screens.erase(iter);
				delete ScreenManager::_removeScreen;
				ScreenManager::_removeScreen = NULL;
			}
		}

		const clock_t clocksPassed = clock() - startClock;
		if(clocksPerFrame > clocksPassed)
		{
			Sleep(clocksPerFrame - clocksPassed);
		}
	}
}