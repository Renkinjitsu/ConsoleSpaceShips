#include "InstructionsScreen.h"

#include "ScreenManager.h"

const char * const InstructionsScreen::_instructions[] =
{
	"\xDB\xDB Keys",
	"\xDB Small spaceship:",
	"\xDB  A - Move left",
	"\xDB  W - Move up",
	"\xDB  D - Move right",
	"\xDB  X - Move down",
	"\xDB  Z - Rotate",
	"\xDB",
	"\xDB Big spaceship:",
	"\xDB  J - Move left",
	"\xDB  I - Move up",
	"\xDB  L - Move right",
	"\xDB  M - Move down",
	"\xDB",
	"\xDB\xDB Objective",
	"\xDB Get the two spaceships (@@, ##) through the exit point (marked by 'X').",
	"\xDB                             ##",
	"\xDB",
	"\xDB\xDB Warning",
	"\xDB Big items may damage the spaceships.",
	"\xDB",
	"\xDB\xDB Tip",
	"\xDB Combine the power of the spaceships to move big items.",
	//"\xDB\xDB\xDB\xDB\xDB\xDB\xDB" TITLE_END,
};

const size_t InstructionsScreen::_instructionsCount = sizeof(_instructions) / sizeof(_instructions[0]);

InstructionsScreen::InstructionsScreen()
{
	unsigned width = 0;

	for(unsigned i = 0; i < InstructionsScreen::_instructionsCount; ++i)
	{
		const unsigned stringLength = (unsigned)strlen(InstructionsScreen::_instructions[i]);
		if(stringLength > width)
		{
			width = stringLength;
		}
	}

	this->_startX = (Canvas::getWidth() - width) / 2;
	this->_startY = (Canvas::getHeight() - InstructionsScreen::_instructionsCount) / 2;
}

InstructionsScreen::~InstructionsScreen()
{
}

void InstructionsScreen::setInitialState()
{
}

void InstructionsScreen::readUserInput(const Keyboard & keyboard)
{
	if(keyboard.isPressed(Keyboard::ESC))
	{
		ScreenManager::remove(this);
	}
}

void InstructionsScreen::process()
{
}

void InstructionsScreen::update()
{
}

void InstructionsScreen::draw(Canvas & canvas) const
{
	for(unsigned i = 0; i < InstructionsScreen::_instructionsCount; ++i)
	{
		unsigned y = this->_startY + InstructionsScreen::_instructionsCount - i;

		canvas.draw(this->_startX, y, InstructionsScreen::_instructions[i]);
	}
}
