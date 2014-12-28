#include "GameConfig.h"

#include <string.h>

const char GameConfig::allowedTextures[] =
{
	GameConfig::TEXTURES_SMALL_SPACESHIP, GameConfig::TEXTURES_BIG_SPACESHIP,
	GameConfig::TEXTURES_WALL, GameConfig::TEXTURES_EXIT,
	GameConfig::TEXTURES_ITEM1, GameConfig::TEXTURES_ITEM2,
	GameConfig::TEXTURES_ITEM3, GameConfig::TEXTURES_ITEM4,
	GameConfig::TEXTURES_ITEM5, GameConfig::TEXTURES_ITEM6,
	GameConfig::TEXTURES_ITEM7, GameConfig::TEXTURES_ITEM8,
	GameConfig::TEXTURES_ITEM9, GameConfig::TEXTURES_EMPTY
};

const unsigned GameConfig::allowedTexturesCount = sizeof(GameConfig::allowedTextures) / sizeof(*GameConfig::allowedTextures);

std::string GameConfig::_levelsPath(""); //Default path for level files (current directory)

const std::string GameConfig::FILE_EXTENSION_LEVEL(".spg");
const std::string GameConfig::FILE_EXTENSION_GAME_SAVE(".spp");
const std::string GameConfig::FILE_EXTENSION_SOLUTION(".sps");

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
