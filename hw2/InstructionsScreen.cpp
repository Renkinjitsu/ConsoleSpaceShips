#include "InstructionsScreen.h"

#include "ScreenManager.h"

InstructionsScreen::InstructionsScreen()
{
	const char * const instructions[] =
	{
		"\xDB\xDB Keys",
		"\xDB Small spaceship:",
		"\xDB  A/W/D/X  Move left/up/right/down",
		"\xDB  Z        Rotate",
		"\xDB",
		"\xDB Big spaceship:",
		"\xDB  J/I/L/M - Move left/up/right/down",
		"\xDB",
		"\xDB\xDB Objective",
		"\xDB Get the two spaceships (@@, ##) through the exit point (marked by 'X').",
		"\xDB                             ##",
		"\xDB",
		"\xDB\xDB Warning",
		"\xDB Big items may damage the spaceships.",
		"\xDB Bombs (marked by '*') detonate by thouch and explode.",
		"\xDB Enemy troops (marked by 'W') are suicidal.",
		"\xDB",
		"\xDB\xDB Tip",
		"\xDB Combine the power of the spaceships to move bigger items."
	};

	const size_t instructionsCount = sizeof(instructions) / sizeof(instructions[0]);

	for(unsigned i = 0; i < instructionsCount; ++i)
	{
		MenuScreen::append(instructions[i]);
	}
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
