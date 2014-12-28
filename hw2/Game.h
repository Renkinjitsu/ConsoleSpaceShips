#ifndef GAME_H_
#define GAME_H_

#include "GameScreen.h"

class Game
{
private:
	Game();
	~Game();

	static unsigned _currentLevelId;
	static GameScreen * _currentGameScreen;
	static std::string _currentLevelFileName;

	static void startLevel();
	static void saveSolution();
public:

	static void start();
	static void startNextLevel();

	static void saveGame();
	static void loadGame();

	static void restart();

	static void gameOver();

	static void exit();
};

#endif /* GAME_H_ */
