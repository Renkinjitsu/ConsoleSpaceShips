#include "GameOverScreen.h"

#include <string.h>

#include "ScreenManager.h"
#include "Keyboard.h"

const char * const GameOverScreen::_endMessage = "Game OVER!";
const size_t GameOverScreen::_endMessageLength = strlen(GameOverScreen::_endMessage);

GameOverScreen::GameOverScreen()
{
	this->_endMessageEffectIndex = 0;
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
	++(this->_endMessageEffectIndex);
	GameOverScreen::_endMessageEffectIndex %= GameOverScreen::_endMessageLength;
}

void GameOverScreen::draw(Canvas & canvas) const
{
	canvas.restore();

	canvas.draw((Canvas::getWidth() - GameOverScreen::_endMessageLength) / 2, Canvas::getHeight() / 2, GameOverScreen::_endMessage);
	canvas.draw((Canvas::getWidth() - GameOverScreen::_endMessageLength) / 2 + this->_endMessageEffectIndex, Canvas::getHeight() / 2, ' ');
}
