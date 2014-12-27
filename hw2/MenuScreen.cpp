#include "MenuScreen.h"

#include "ScreenManager.h"

MenuScreen::MenuScreen() : _drawOffset(Canvas::CENTER)
{
	this->_maxLength = 0;
}

MenuScreen::~MenuScreen()
{
}

unsigned MenuScreen::getRowsCount() const
{
	return this->_menuItems.size();
}

void MenuScreen::append(const char * const line)
{
	this->_menuItems.push_back(line);

	//For every 2 columns, start drawing 1 column to the left (keeps the complete text block centered)
	const unsigned length = (unsigned)strlen(line);
	if(length > this->_maxLength)
	{
		unsigned factor = length - this->_maxLength;
		if((this->_maxLength % 2) > 0) //We lost the 0.5 in the previous calculation
		{
			factor++;
		}
		factor /= 2; //Correction is half of to the left, the other half will go to the right

		Point leftCorrection(Point::LEFT);
		leftCorrection.multiply(factor);

		this->_drawOffset.move(leftCorrection);

		this->_maxLength = length;
	}

	//For every 2 rows, start drawing 1 row higher (keeps the complete text block centered)
	if((this->getRowsCount() % 2) == 0) //The rows count is now even, we lost 0.5 in the previous calculation
	{
		this->_drawOffset.move(Point::UP);
	}
}

void MenuScreen::append(const std::string & line)
{
	this->append(line.c_str());
}

void MenuScreen::setInitialState()
{
}

void MenuScreen::readUserInput(const Keyboard & keyboard)
{
	if(keyboard.isPressed(Keyboard::ESC))
	{
		ScreenManager::remove(this);
	}
}

void MenuScreen::process()
{
}

void MenuScreen::update()
{
}

void MenuScreen::draw(Canvas & canvas) const
{
	Point startPosition(this->_drawOffset);

	for(unsigned i = 0; i < this->_menuItems.size(); ++i, startPosition.move(Point::DOWN))
	{
		canvas.draw(startPosition, this->_menuItems[i]);
	}
}
