#include "ErrorsScreen.h"

#include "ScreenManager.h"

ErrorsScreen::ErrorsScreen()
{
	this->_width = 0;
}

ErrorsScreen::~ErrorsScreen()
{
}

void ErrorsScreen::updateStartPoint()
{
	this->_startX = (Canvas::getWidth() - this->_width) / 2;
	this->_startY = (Canvas::getHeight() - this->_errors.size()) / 2;
}

void ErrorsScreen::append(const std::string & error)
{
	this->_errors.push_back(error);

	if(error.length() > this->_width)
	{
		this->_width = error.length();

		this->updateStartPoint();
	}
}

void ErrorsScreen::setInitialState()
{
}

void ErrorsScreen::readUserInput(const Keyboard & keyboard)
{
	if(keyboard.isPressed(Keyboard::ESC))
	{
		ScreenManager::remove(this);
	}
}

void ErrorsScreen::process()
{
}

void ErrorsScreen::update()
{
}

void ErrorsScreen::draw(Canvas & canvas) const
{
	for(unsigned i = 0; i < this->_errors.size(); ++i)
	{
		unsigned y = this->_startY + this->_errors.size() - i;

		canvas.draw(this->_startX, y, this->_errors[i]);
	}
}
