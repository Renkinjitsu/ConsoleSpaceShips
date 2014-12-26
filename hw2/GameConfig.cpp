#include "GameConfig.h"

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

void GameConfig::getLevelsPath(const char * const newPath)
{
	GameConfig::_levelsPath = newPath;
}
