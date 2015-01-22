#include "GameOverScreen.h"

#include <string.h>

#include "ScreenManager.h"
#include "Keyboard.h"

const char * const GameOverScreen::_endMessage = "Game OVER!";
const size_t GameOverScreen::_endMessageLength = strlen(GameOverScreen::_endMessage);

GameOverScreen::GameOverScreen()
{
	_highlightEndPosition = _highlightPosition = _drawOffset =
		Canvas::CENTER + (Point::LEFT * (GameOverScreen::_endMessageLength / 2));

	_highlightEndPosition += Point::RIGHT * (GameOverScreen::_endMessageLength);
}

GameOverScreen::~GameOverScreen()
{
}

void GameOverScreen::setInitialState()
{
}

void GameOverScreen::readUserInput(const Keyboard & keyboard)
{
	if(keyboard.isPressed(keyboard.ESC))
	{
		ScreenManager::remove(this);
	}
}

void GameOverScreen::process()
{
}

void GameOverScreen::update()
{
	_highlightPosition += Point::RIGHT;

	if(_highlightPosition == _highlightEndPosition)
	{
		_highlightPosition = _drawOffset;
	}
}

void GameOverScreen::draw(Canvas & canvas) const
{
	canvas.restore();

	canvas.draw(_drawOffset, GameOverScreen::_endMessage);
	canvas.draw(_highlightPosition, GameOverScreen::_highlightSymbol);
}
