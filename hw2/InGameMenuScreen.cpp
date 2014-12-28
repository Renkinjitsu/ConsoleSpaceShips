#include "InGameMenuScreen.h"

#include "Game.h"
#include "ScreenManager.h"

InGameMenuScreen::InGameMenuScreen()
{
	this->append("1. Restart level");
	this->append("2. Save and exit to menu");
	this->append("3. View level solution");
	this->append("");
	this->append("8. Exit to main menu");
	this->append("9. Quit");
}


InGameMenuScreen::~InGameMenuScreen()
{
}

void InGameMenuScreen::setInitialState()
{
}

void InGameMenuScreen::readUserInput(const Keyboard & keyboard)
{
	if(keyboard.isPressed(Keyboard::ESC))
	{
		ScreenManager::remove(this);
	}
	if(keyboard.isPressed(Keyboard::NUM1))
	{
		Game::restart();
		ScreenManager::remove(this);
	}
	else if(keyboard.isPressed(Keyboard::NUM2))
	{
		Game::saveGame();
		Game::exit();
		ScreenManager::remove(this);
	}
	else if(keyboard.isPressed(Keyboard::NUM3))
	{
		//TODO: Implement
		//ScreenManager::add(solutionViewerScreen);
	}
	else if(keyboard.isPressed(Keyboard::NUM8))
	{
		Game::exit();
		ScreenManager::remove(this);
	}
	else if(keyboard.isPressed(Keyboard::NUM9))
	{
		ScreenManager::removeAll();
	}
}

void InGameMenuScreen::process()
{
}

void InGameMenuScreen::update()
{
}
