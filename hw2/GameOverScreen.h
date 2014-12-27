#ifndef GAME_OVER_SCREEN_H_
#define GAME_OVER_SCREEN_H_

#include "Screen.h"

class GameOverScreen : public Screen
{
private:
	static const char * const _endMessage;
	static const size_t _endMessageLength;
	static const char _highlightSymbol = (char)219;

	Point _drawOffset;
	Point _highlightPosition;
	Point _highlightEndPosition;

public:
	GameOverScreen();
	~GameOverScreen();

	void setInitialState();
	void readUserInput(const Keyboard & keyboard);
	void process();
	void update();
	void draw(Canvas & canvas) const;
};

#endif /* GAME_OVER_SCREEN_H_ */
