#ifndef REPLAY_SCREEN_H_
#define REPLAY_SCREEN_H_

#include "GameScreen.h"

class ReplayScreen : GameScreen
{
private:
	const GameRecorder & _replyRecored;

public:
	ReplayScreen(const GameRecorder & replyRecored);
	~ReplayScreen();
};

#endif /* REPLAY_SCREEN_H_ */
