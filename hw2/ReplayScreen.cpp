#include "ReplayScreen.h"

#include <iostream>
#include <fstream>

#include "GameScreenBuilder.h"
#include "FilesManager.h"

ReplayScreen::ReplayScreen(const std::string & levelName)
{
	this->_currentIteration = 0;

	this->_solutionFile = FilesManager::openFile(levelName, FilesManager::FILE_TYPE_SOLUTION);

	GameScreenBuilder builder;
	builder.loadFromFile(levelName);
	this->_gameScreen = (GameScreen *)builder.build();

	GameScreen & screen = *this->_gameScreen;

	std::string line;
	std::getline(*this->_solutionFile, line); //Skip Screen ID
	std::getline(*this->_solutionFile, line); //Skip name of solver

	this->nextRecored._parametersCount = 0;
}

ReplayScreen::~ReplayScreen()
{
	this->_solutionFile->close();
	delete this->_solutionFile;
}

void ReplayScreen::setInitialState()
{
	this->_gameScreen->setInitialState();
}

void ReplayScreen::readUserInput(const Keyboard & keyboard)
{
	if(this->nextRecored._parametersCount == 0 &&
		this->_solutionFile->eof() == false)
	{
		std::string line;
		std::getline(*this->_solutionFile, line);
		this->nextRecored._parametersCount = sscanf(line.c_str(), "%u: %c%c", &this->nextRecored._id,
			&this->nextRecored._key1, &this->nextRecored._key2);
	}

	if(this->nextRecored._parametersCount >= 2 &&
		this->nextRecored._id == this->_currentIteration)
	{
		Keyboard dummyKeyboard;
		dummyKeyboard.setPress(this->nextRecored._key1);
		if(this->nextRecored._parametersCount >= 3)
		{
			dummyKeyboard.setPress(this->nextRecored._key2);
		}

		this->_gameScreen->readUserInput(dummyKeyboard);

		this->nextRecored._parametersCount = 0;
	}

	++this->_currentIteration;
}

void ReplayScreen::process()
{
	this->_gameScreen->process();
}
void ReplayScreen::update()
{
	this->_gameScreen->update();
}

void ReplayScreen::draw(Canvas & canvas) const
{
	this->_gameScreen->draw(canvas);
}
