#include "Keyboard.h"

#include "io_utils.h"
#include <cctype>

void Keyboard::resetPressedKeys()
{
	for(size_t i = 0; i < Keyboard::SUPPORTED_KEYS; ++i)
	{
		this->_pressedKeys[i] = false;
	}
}

Keyboard::Keyboard()
{
	this->resetPressedKeys();
}

Keyboard::~Keyboard()
{
}

void Keyboard::update()
{
	this->resetPressedKeys();

	while(_kbhit())
	{
		char ch = _getch();

		if(ch == '\r')
		{
			ch = '\n';
		}

		ch = tolower(ch);

		if(ch < Keyboard::SUPPORTED_KEYS)
		{
			this->_pressedKeys[ch] = true;
		}
	}
}

bool Keyboard::isPressed(Key key) const
{
	return this->_pressedKeys[key];
}
