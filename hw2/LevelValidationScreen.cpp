#include "LevelValidationScreen.h"

#include <algorithm>

#include "FilesManager.h"
#include "ScreenManager.h"
#include "MenuScreen.h"

const Keyboard::Key LevelValidationScreen::numberKeys[] =
{
	Keyboard::NUM1, Keyboard::NUM2, Keyboard::NUM3,
	Keyboard::NUM4, Keyboard::NUM5, Keyboard::NUM6,
	Keyboard::NUM7, Keyboard::NUM8, Keyboard::NUM9
};

const unsigned LevelValidationScreen::MAX_SUPPORTED_FILES = (sizeof(LevelValidationScreen::numberKeys) / sizeof(*LevelValidationScreen::numberKeys));

LevelValidationScreen::LevelValidationScreen()
{
	const std::vector<std::string> availableFileNames = FilesManager::getFileNames();

	//Support only first 'MAX_SUPPORTED_FILES' files
	const unsigned relevantFilesCount = std::min(availableFileNames.size(), LevelValidationScreen::MAX_SUPPORTED_FILES);

	char indexingCache[] = {'0', ')', ' ', '\0'};

	for(unsigned i = 0; i < relevantFilesCount; ++i)
	{
		++(indexingCache[0]);

		unsigned screenId;
		bool isValidScreenId = FilesManager::getScreenId(GameConfig::getLevelsPath() + availableFileNames[i], screenId);

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
		for(unsigned i = 0; i < this->_options.size(); ++i)
		{
			if(keyboard.isPressed(numberKeys[i]))
			{
				selectedIndex = i;
				optionSelected = true;
			}
		}

		if(optionSelected)
		{
			const std::string selectedFile = GameConfig::getLevelsPath() + this->_fileNames[selectedIndex];

			this->_builder.loadFromFile(selectedFile);
			if(this->_builder.isValid())
			{
				MenuScreen * message = new MenuScreen();
				message->append("No errors detected, the file is valid!");
				ScreenManager::add(message);
			}
			else
			{
				ScreenManager::add(this->_builder.build());
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
		canvas.draw(startPosition, "Choose a file to test:");
		startPosition.move(Point::RIGHT); //Ident
		startPosition.move(Point::DOWN); //Move 1 row lower

		for(unsigned i = 0; i < filesCount; ++i)
		{
			startPosition.move(Point::DOWN); //Move 1 row lower
			canvas.draw(startPosition, this->_options[i]);
		}
	}
}
