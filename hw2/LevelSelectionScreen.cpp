#include "LevelSelectionScreen.h"

#include <algorithm>

#include "FilesManager.h"
#include "ScreenManager.h"
#include "Game.h"
#include "GameScreenBuilder.h"

const unsigned LevelSelectionScreen::MAX_SUPPORTED_FILES = Keyboard::numberKeysCount;

LevelSelectionScreen::LevelSelectionScreen()
{
	const std::vector<std::string> availableFileNames = FilesManager::getFilesList(FilesManager::FileType::FILE_TYPE_LEVEL);

	//Support only first 'MAX_SUPPORTED_FILES' files
	const unsigned relevantFilesCount = std::min(availableFileNames.size(), LevelSelectionScreen::MAX_SUPPORTED_FILES);

	char indexingCache[] = {'0', ')', ' ', '\0'};

	for(unsigned i = 0; i < relevantFilesCount; ++i)
	{
		++(indexingCache[0]);

		unsigned screenId;
		bool isValidScreenId = FilesManager::getScreenId(availableFileNames[i], screenId);

		std::string option;
		option += indexingCache;
		option += availableFileNames[i];

		if(isValidScreenId)
		{
			option += " (Screen ID \'";
			option += std::to_string(screenId);
			option += "\')";
		}
		else
		{
			option += " (Currupted screen ID)";
		}

		this->_fileNames.push_back(availableFileNames[i]);
		this->_options.push_back(option);
	}
}

LevelSelectionScreen::~LevelSelectionScreen()
{
}

void LevelSelectionScreen::setInitialState()
{
}

void LevelSelectionScreen::readUserInput(const Keyboard & keyboard)
{
	if(keyboard.isPressed(Keyboard::ESC))
	{
		ScreenManager::remove(this);
	}
	else
	{
		bool optionSelected = false;
		unsigned selectedIndex;
		for(unsigned i = 0; i < this->_options.size(); ++i)
		{
			if(keyboard.isPressed(Keyboard::numberKeys[i]))
			{
				selectedIndex = i;
				optionSelected = true;
			}
		}

		if(optionSelected)
		{
			unsigned screenId;
			bool isValidScreenId = FilesManager::getScreenId(this->_fileNames[selectedIndex], screenId);

			if(isValidScreenId)
			{
				ScreenManager::remove(this);
				Game::start(screenId);
			}
			else
			{
				GameScreenBuilder builder;
				builder.loadFromFile(this->_fileNames[selectedIndex]);
				ScreenManager::add(builder.build());
			}
		}
	}
}

void LevelSelectionScreen::process()
{
}

void LevelSelectionScreen::update()
{
}

void LevelSelectionScreen::draw(Canvas & canvas) const
{
	Point startPosition(Canvas::TOP_LEFT);
	startPosition.move(Point::RIGHT); //Padding from the left
	startPosition.move(Point::DOWN); //Padding from the right

	const unsigned filesCount = (unsigned)this->_options.size();

	if(filesCount == 0)
	{
		canvas.draw(startPosition, "No files are available");
	}
	else
	{
		canvas.draw(startPosition, "Choose a file to play:");
		startPosition.move(Point::RIGHT); //Ident
		startPosition.move(Point::DOWN); //Move 1 row lower

		for(unsigned i = 0; i < filesCount; ++i)
		{
			startPosition.move(Point::DOWN); //Move 1 row lower
			canvas.draw(startPosition, this->_options[i]);
		}
	}
}
