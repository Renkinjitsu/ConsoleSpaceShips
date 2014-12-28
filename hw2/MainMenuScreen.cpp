#include "MainMenuScreen.h"

#include <string.h>

#include "Game.h"
#include "ScreenManager.h"
#include "InstructionsScreen.h"
#include "LevelValidationScreen.h"

MainMenuScreen::MainMenuScreen()
{
	this->append("1. New game");
	this->append("2. Instructions");
	this->append("3. Validate level file");
	this->append("4. Load saved game");
	this->append("5. Start specific level");
	this->append("");
	this->append("9. Exit");
}

MainMenuScreen::~MainMenuScreen()
{
}

void MainMenuScreen::setInitialState()
{
}

void MainMenuScreen::readUserInput(const Keyboard & keyboard)
{
	if(keyboard.isPressed(Keyboard::NUM1))
	{
		Game::start();
	}
	else if(keyboard.isPressed(Keyboard::NUM2))
	{
		ScreenManager::add(new InstructionsScreen());
	}
	else if(keyboard.isPressed(Keyboard::NUM3))
	{
		ScreenManager::add(new LevelValidationScreen());
	}
	else if(keyboard.isPressed(Keyboard::NUM4))
	{
		Game::loadGame();
	}
	else if(keyboard.isPressed(Keyboard::NUM5))
	{
		//TODO: Implement
		//ScreenManager::add(new LoadSpecificLevelScreen());
	}
	else if(keyboard.isPressed(Keyboard::NUM9))
	{
		ScreenManager::remove(this);
	}
}

void MainMenuScreen::process()
{
}

void MainMenuScreen::update()
{
}
