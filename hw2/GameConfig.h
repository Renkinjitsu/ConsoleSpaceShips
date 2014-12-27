#ifndef GAME_CONFIG_H_
#define GAME_CONFIG_H_

#include <string>

class GameConfig
{
private:
	GameConfig();
	~GameConfig();

	static std::string _levelsPath;

public:

	static const unsigned SCREEN_WIDTH = 80;
	static const unsigned SCREEN_HEIGHT = 24;

	static const char TEXTURES_WALL = '+';
	static const char TEXTURES_EXIT = 'X';
	static const char TEXTURES_SMALL_SPACESHIP = '@';
	static const char TEXTURES_BIG_SPACESHIP = '#';
	static const char TEXTURES_ITEM1 = '1';
	static const char TEXTURES_ITEM2 = '2';
	static const char TEXTURES_ITEM3 = '3';
	static const char TEXTURES_ITEM4 = '4';
	static const char TEXTURES_ITEM5 = '5';
	static const char TEXTURES_ITEM6 = '6';
	static const char TEXTURES_ITEM7 = '7';
	static const char TEXTURES_ITEM8 = '8';
	static const char TEXTURES_ITEM9 = '9';
	static const char TEXTURES_EMPTY = ' ';

	static const char allowedTextures[];
	static const unsigned allowedTexturesCount;

	static const std::string & getLevelsPath();
	static void setLevelsPath(const char * const newPath);
};

#endif /* GAME_CONFIG_H_ */
