#include "ReplayScreen.h"

#include <iostream>
#include <fstream>

#include "GameScreenBuilder.h"
#include "FilesManager.h"
#include "ScreenManager.h"

ReplayScreen::ReplayScreen(const std::string & levelName)
{
	_currentIteration = 0;

	_solutionFile = FilesManager::openFile(levelName, FilesManager::FILE_TYPE_SOLUTION);

	GameScreenBuilder builder;
	builder.loadFromFile(levelName);
	_gameScreen = (GameScreen *)builder.build();

	GameScreen & screen = *_gameScreen;

	std::string line;
	std::getline(*_solutionFile, line); //Skip Screen ID
	std::getline(*_solutionFile, line); //Skip name of solver

	this->nextRecored._parametersCount = 0;
}

ReplayScreen::~ReplayScreen()
{
	_solutionFile->close();
	delete _solutionFile;
}

void ReplayScreen::setInitialState()
{
	_gameScreen->setInitialState();
}

void ReplayScreen::readUserInput(const Keyboard & keyboard)
{
	if(keyboard.isPressed(Keyboard::Key::ESC))
	{
		ScreenManager::remove(this);
	}

	if(this->nextRecored._parametersCount == 0 &&
		_solutionFile->eof() == false)
	{
		std::string line;
		std::getline(*_solutionFile, line);
		this->nextRecored._parametersCount = sscanf(line.c_str(), "%u: %c%c", &this->nextRecored._id,
			&this->nextRecored._key1, &this->nextRecored._key2);
	}

	if(this->nextRecored._parametersCount >= 2 &&
		this->nextRecored._id == _currentIteration)
	{
		Keyboard dummyKeyboard;
		dummyKeyboard.setPress(this->nextRecored._key1);
		if(this->nextRecored._parametersCount >= 3)
		{
			dummyKeyboard.setPress(this->nextRecored._key2);
		}

		_gameScreen->readUserInput(dummyKeyboard);

		this->nextRecored._parametersCount = 0;
	}

	++_currentIteration;
}

void ReplayScreen::process()
{
	_gameScreen->process();
}
void ReplayScreen::update()
{
	_gameScreen->update();
}

void ReplayScreen::draw(Canvas & canvas) const
{
	_gameScreen->draw(canvas);
}
