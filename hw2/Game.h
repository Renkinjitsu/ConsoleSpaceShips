#ifndef GAME_H_
#define GAME_H_

class Game
{
private:
	Game();
	~Game();

	static unsigned _nextLevelId;

	static void startLevel();
public:

	static void start();
	static void startNextLevel();
	static void gameOver();
};

#endif /* GAME_H_ */
