#include "MenuScreen.h"

#include "ScreenManager.h"

MenuScreen::MenuScreen() : _drawOffset(Canvas::CENTER)
{
	_maxLength = 0;
}

MenuScreen::~MenuScreen()
{
}

unsigned MenuScreen::getRowsCount() const
{
	return _menuItems.size();
}

void MenuScreen::append(const char * const line)
{
	_menuItems.push_back(line);

	//For every 2 columns, start drawing 1 column to the left (keeps the complete text block centered)
	const unsigned length = (unsigned)strlen(line);
	if(length > _maxLength)
	{
		unsigned factor = length - _maxLength;
		if((_maxLength % 2) > 0) //We lost the 0.5 in the previous calculation
		{
			factor++;
		}
		factor /= 2; //Correction is half of to the left, the other half will go to the right

		_drawOffset += Point::LEFT * factor;

		_maxLength = length;
	}

	//For every 2 rows, start drawing 1 row higher (keeps the complete text block centered)
	if((this->getRowsCount() % 2) == 0) //The rows count is now even, we lost 0.5 in the previous calculation
	{
		_drawOffset += Point::UP;
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
	Point startPosition(_drawOffset);

	for(unsigned i = 0; i < _menuItems.size(); ++i, startPosition += Point::DOWN)
	{
		canvas.draw(startPosition, _menuItems[i]);
	}
}
