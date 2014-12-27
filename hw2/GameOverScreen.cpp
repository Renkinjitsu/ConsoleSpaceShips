#include "GameOverScreen.h"

#include <string.h>

#include "ScreenManager.h"
#include "Keyboard.h"

const char * const GameOverScreen::_endMessage = "Game OVER!";
const size_t GameOverScreen::_endMessageLength = strlen(GameOverScreen::_endMessage);

GameOverScreen::GameOverScreen()
{
	Point correction;

	this->_drawOffset = Canvas::CENTER;
	correction = Point::LEFT;
	correction.multiply(GameOverScreen::_endMessageLength / 2);
	this->_drawOffset.move(correction);

	this->_highlightPosition = this->_drawOffset;

	this->_highlightEndPosition = this->_drawOffset;
	correction = Point::RIGHT;
	correction.multiply(GameOverScreen::_endMessageLength);
	this->_highlightEndPosition.move(correction);
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
	if(this->_highlightPosition.equals(this->_highlightEndPosition))
	{
		this->_highlightPosition = this->_drawOffset;
	}
	else
	{
		this->_highlightPosition.move(Point::RIGHT);
	}
}

void GameOverScreen::draw(Canvas & canvas) const
{
	canvas.restore();

	canvas.draw(this->_drawOffset, GameOverScreen::_endMessage);
	canvas.draw(this->_highlightPosition, GameOverScreen::_highlightSymbol);
}
