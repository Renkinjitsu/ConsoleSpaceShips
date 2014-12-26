#include "GameConfig.h"

#include <string.h>

std::string GameConfig::_levelsPath(""); //Default path for level files (current directory)

GameConfig::GameConfig()
{
}

GameConfig::~GameConfig()
{
}

const std::string & GameConfig::getLevelsPath()
{
	return GameConfig::_levelsPath;
}

void GameConfig::setLevelsPath(const char * const newPath)
{
	GameConfig::_levelsPath = newPath;

	//Verify that the path ends with a folder seperator ('\' or '/')
	if(GameConfig::_levelsPath.length() > 0)
	{
		const char lastCharacter = *(GameConfig::_levelsPath.end() - 1);

		const size_t seperatorIndex = GameConfig::_levelsPath.find_first_of('/');
		const char folderSeperator = (seperatorIndex == std::string::npos) ? '\\' : '/';

		if(lastCharacter != folderSeperator)
		{
			GameConfig::_levelsPath += folderSeperator;
		}
	}
}
