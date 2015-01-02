#ifndef REPLAY_SCREEN_H_
#define REPLAY_SCREEN_H_

#include <string>

#include "Screen.h"
#include "GameScreen.h"

class ReplayScreen : public Screen
{
private:
	std::ifstream * _solutionFile;

	GameScreen * _gameScreen;
	unsigned _currentIteration;

	struct
	{
		unsigned _id;
		char _key1;
		char _key2;

		int _parametersCount;
	}nextRecored;

public:
	ReplayScreen(const std::string & levelName);
	~ReplayScreen();

	void setInitialState();
	void readUserInput(const Keyboard & keyboard);
	void process();
	void update();
	void draw(Canvas & canvas) const;
};

#endif /* REPLAY_SCREEN_H_ */
