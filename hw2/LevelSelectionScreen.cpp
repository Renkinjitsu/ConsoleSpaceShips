#include "LevelSelectionScreen.h"

#include <algorithm>

#include "FilesManager.h"
#include "ScreenManager.h"
#include "Game.h"
#include "GameScreenBuilder.h"

const unsigned LevelSelectionScreen::MAX_SUPPORTED_FILES = Keyboard::numberKeysCount;

const std::string & LevelSelectionScreen::getLevelName(unsigned id, const std::vector<std::string> & levels)
{
	for(unsigned i = 0; i < levels.size(); ++i)
	{
		unsigned levelId;
		if(FilesManager::getScreenId(levels[i], FilesManager::FileType::FILE_TYPE_LEVEL, levelId))
		{
			if(levelId == id)
			{
				return levels[i];
			}
		}
	}

	return levels[0]; //Just in case, though shouldn't happen
}

LevelSelectionScreen::LevelSelectionScreen(LevelSelectionScreen::LoadType loadType)
{
	const FilesManager::FileType fileType = (loadType == LevelSelectionScreen::LoadType::LOAD_SAVED_GAME)
		? FilesManager::FileType::FILE_TYPE_SAVE : FilesManager::FileType::FILE_TYPE_LEVEL;

	_loadType = loadType;

	const std::vector<std::string> newGameFileNames = FilesManager::getFilesList(FilesManager::FileType::FILE_TYPE_LEVEL);
	std::vector<std::string> savedGameFileNames;
	if(loadType == LevelSelectionScreen::LoadType::LOAD_SAVED_GAME)
	{
		savedGameFileNames = FilesManager::getFilesList(FilesManager::FileType::FILE_TYPE_SAVE);
	}

	const std::vector<std::string> & availableFileNames = (loadType == LevelSelectionScreen::LoadType::LOAD_SAVED_GAME) ?
		savedGameFileNames : newGameFileNames;

	//Support only first 'MAX_SUPPORTED_FILES' files
	const unsigned relevantFilesCount = std::min(availableFileNames.size(), LevelSelectionScreen::MAX_SUPPORTED_FILES);

	char indexingCache[] = {'0', ')', ' ', '\0'};

	for(unsigned i = 0; i < relevantFilesCount; ++i)
	{
		++(indexingCache[0]);

		unsigned screenId;
		bool isValidScreenId = FilesManager::getScreenId(availableFileNames[i], fileType, screenId);

		std::string option;
		option += indexingCache;
		option += availableFileNames[i];

		if(isValidScreenId)
		{
			option += " (Screen ID \'";
			option += std::to_string(screenId);
			option += '\'';

			if(loadType == LevelSelectionScreen::LoadType::LOAD_SAVED_GAME)
			{
				option += ", Level: ";
				option += LevelSelectionScreen::getLevelName(screenId, newGameFileNames);
			}

			option += ')';
		}
		else
		{
			option += " (Currupted screen ID)";
		}

		_fileNames.push_back(availableFileNames[i]);
		_options.push_back(option);
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
			const FilesManager::FileType fileType = (_loadType == LevelSelectionScreen::LoadType::LOAD_SAVED_GAME)
				? FilesManager::FileType::FILE_TYPE_SAVE : FilesManager::FileType::FILE_TYPE_LEVEL;

			unsigned screenId;
			const bool isValidScreenId = FilesManager::getScreenId(_fileNames[selectedIndex], fileType, screenId);

			if(isValidScreenId)
			{
				if(_loadType == LevelSelectionScreen::LOAD_NEW_GAME)
				{
					Game::start(screenId);
				}
				else
				{
					Game::loadGame(_fileNames[selectedIndex], screenId);
				}
				ScreenManager::remove(this);
			}
			else
			{
				GameScreenBuilder builder;
				builder.loadFromFile(_fileNames[selectedIndex]);
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
		canvas.draw(startPosition, "Choose a game to play:");
		startPosition += Point::RIGHT; //Ident
		startPosition += Point::DOWN; //Move 1 row lower

		for(unsigned i = 0; i < filesCount; ++i)
		{
			startPosition += Point::DOWN; //Move 1 row lower
			canvas.draw(startPosition, _options[i]);
		}
	}
}
