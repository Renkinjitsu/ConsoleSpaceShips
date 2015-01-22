#include "LevelValidationScreen.h"

#include <algorithm>

#include "FilesManager.h"
#include "ScreenManager.h"
#include "MenuScreen.h"

const unsigned LevelValidationScreen::MAX_SUPPORTED_FILES = Keyboard::numberKeysCount;

LevelValidationScreen::LevelValidationScreen()
{
	const std::vector<std::string> availableFileNames = FilesManager::getFilesList(FilesManager::FileType::FILE_TYPE_LEVEL);

	//Support only first 'MAX_SUPPORTED_FILES' files
	const unsigned relevantFilesCount = std::min(availableFileNames.size(), LevelValidationScreen::MAX_SUPPORTED_FILES);

	char indexingCache[] = {'0', ')', ' ', '\0'};

	for(unsigned i = 0; i < relevantFilesCount; ++i)
	{
		++(indexingCache[0]);

		unsigned screenId;
		bool isValidScreenId = FilesManager::getScreenId(availableFileNames[i], FilesManager::FileType::FILE_TYPE_LEVEL, screenId);

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

		_fileNames.push_back(availableFileNames[i]);
		_options.push_back(option);
	}
}

LevelValidationScreen::~LevelValidationScreen()
{
}

void LevelValidationScreen::setInitialState()
{
}

void LevelValidationScreen::readUserInput(const Keyboard & keyboard)
{
	if(keyboard.isPressed(Keyboard::ESC))
	{
		ScreenManager::remove(this);
	}
	else
	{
		bool optionSelected = false;
		unsigned selectedIndex;
		for(unsigned i = 0; i < _options.size(); ++i)
		{
			if(keyboard.isPressed(Keyboard::numberKeys[i]))
			{
				selectedIndex = i;
				optionSelected = true;
			}
		}

		if(optionSelected)
		{
			_builder.loadFromFile(_fileNames[selectedIndex]);
			if(_builder.isValid())
			{
				MenuScreen * message = new MenuScreen();
				message->append("No errors detected, the file is valid!");
				ScreenManager::add(message);
			}
			else
			{
				ScreenManager::add(_builder.build());
			}
		}
	}
}

void LevelValidationScreen::process()
{
}

void LevelValidationScreen::update()
{
}

void LevelValidationScreen::draw(Canvas & canvas) const
{
	Point startPosition = Canvas::TOP_LEFT +
		Point::RIGHT + //Padding from the left
		Point::DOWN; //Padding from the right

	const unsigned filesCount = (unsigned)_options.size();

	if(filesCount == 0)
	{
		canvas.draw(startPosition, "No files are available");
	}
	else
	{
		canvas.draw(startPosition, "Choose a file to test:");
		startPosition += Point::RIGHT; //Ident
		startPosition += Point::DOWN; //Move 1 row lower

		for(unsigned i = 0; i < filesCount; ++i)
		{
			startPosition += Point::DOWN; //Move 1 row lower
			canvas.draw(startPosition, _options[i]);
		}
	}
}
