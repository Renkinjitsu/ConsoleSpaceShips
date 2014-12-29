#include "GameRecorder.h"

GameIterationRecorder::GameIterationRecorder(unsigned id)
{
	this->_id = id;
}

GameIterationRecorder::~GameIterationRecorder()
{
}

unsigned GameIterationRecorder::getId() const
{
	return this->_id;
}

void GameIterationRecorder::recored(char pressedCharacter)
{
	this->_pressedCharacters.push_back(pressedCharacter);
}

GameIterationRecorder::const_iterator GameIterationRecorder::cbegin()
{
	return this->_pressedCharacters.cbegin();
}

GameIterationRecorder::const_iterator GameIterationRecorder::cend()
{
	return this->_pressedCharacters.cend();
}

GameRecorder::GameRecorder()
{
}

GameRecorder::~GameRecorder()
{
}

void GameRecorder::recored(unsigned iterationId, char pressedKey)
{
	if(this->_iterationRecords.empty() ||
		(this->_iterationRecords[this->_iterationRecords.size() - 1].getId() != iterationId))
	{
		this->_iterationRecords.push_back(iterationId);
	}

	this->_iterationRecords[this->_iterationRecords.size() - 1].recored(pressedKey);
}

GameRecorder::const_iterator GameRecorder::cbegin()
{
	return this->_iterationRecords.cbegin();
}

GameRecorder::const_iterator GameRecorder::cend()
{
	return this->_iterationRecords.cend();
}
