#include "MainMenuScreen.h"

#include <string.h>

#include "Game.h"
#include "ScreenManager.h"
#include "InstructionsScreen.h"

MainMenuScreen::MainMenuScreen()
{
	this->_menuItems.push_back("1. New game");
	this->_menuItems.push_back("2. Instructions");
	this->_menuItems.push_back("3. Validate level file");
	this->_menuItems.push_back("4. Load saved game");
	this->_menuItems.push_back("5. Start specific level");
	this->_menuItems.push_back("");
	this->_menuItems.push_back("9. Exit");

	unsigned width = 0;
	for(unsigned i = 0; i < this->_menuItems.size(); ++i)
	{
		const unsigned stringLength = (unsigned)strlen(this->_menuItems[i]);
		if(stringLength > width)
		{
			width = stringLength;
		}
	}

	this->_startX = (Canvas::getWidth() - width) / 2;
	this->_startY = (Canvas::getHeight() - this->_menuItems.size()) / 2;
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
		//ScreenManager::add(new LevelValidationScreen());
	}
	else if(keyboard.isPressed(Keyboard::NUM4))
	{
		//ScreenManager::add(new LoadSavedGameScreen());
	}
	else if(keyboard.isPressed(Keyboard::NUM5))
	{
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

void MainMenuScreen::draw(Canvas & canvas) const
{
	for(unsigned i = 0; i < this->_menuItems.size(); ++i)
	{
		unsigned y = this->_startY + this->_menuItems.size() - i;

		canvas.draw(this->_startX, y, this->_menuItems[i]);
	}
}
