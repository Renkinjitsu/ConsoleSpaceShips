#include "GameConfig.h"

#include <string.h>

const char GameConfig::allowedTextures[] =
{
	GameConfig::TEXTURES_SMALL_SPACESHIP,
	GameConfig::TEXTURES_BIG_SPACESHIP,
	GameConfig::TEXTURES_BAD_SPACESHIP,
	GameConfig::TEXTURES_BOMB,
	GameConfig::TEXTURES_WALL,
	GameConfig::TEXTURES_EXIT,
	GameConfig::TEXTURES_ITEM1,
	GameConfig::TEXTURES_ITEM2,
	GameConfig::TEXTURES_ITEM3,
	GameConfig::TEXTURES_ITEM4,
	GameConfig::TEXTURES_ITEM5,
	GameConfig::TEXTURES_ITEM6,
	GameConfig::TEXTURES_ITEM7,
	GameConfig::TEXTURES_ITEM8,
	GameConfig::TEXTURES_ITEM9,
	GameConfig::TEXTURES_EMPTY
};

const unsigned GameConfig::allowedTexturesCount = sizeof(GameConfig::allowedTextures) / sizeof(*GameConfig::allowedTextures);
