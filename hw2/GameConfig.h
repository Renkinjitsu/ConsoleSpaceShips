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

	static const std::string & getLevelsPath();
	static void setLevelsPath(const char * const newPath);
};

#endif /* GAME_CONFIG_H_ */
