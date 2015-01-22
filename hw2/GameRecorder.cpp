#include "GameRecorder.h"

GameIterationRecorder::GameIterationRecorder(unsigned id)
{
	_id = id;
}

GameIterationRecorder::~GameIterationRecorder()
{
}

unsigned GameIterationRecorder::getId() const
{
	return _id;
}

void GameIterationRecorder::recored(char pressedCharacter)
{
	_pressedCharacters.push_back(pressedCharacter);
}

GameIterationRecorder::const_iterator GameIterationRecorder::cbegin() const
{
	return _pressedCharacters.cbegin();
}

GameIterationRecorder::const_iterator GameIterationRecorder::cend() const
{
	return _pressedCharacters.cend();
}

GameRecorder::GameRecorder()
{
}

GameRecorder::~GameRecorder()
{
}

void GameRecorder::recored(unsigned iterationId, char pressedKey)
{
	if(_iterationRecords.empty() ||
		(_iterationRecords[_iterationRecords.size() - 1].getId() != iterationId))
	{
		_iterationRecords.push_back(iterationId);
	}

	_iterationRecords[_iterationRecords.size() - 1].recored(pressedKey);
}

GameRecorder::const_iterator GameRecorder::cbegin() const
{
	return _iterationRecords.cbegin();
}

GameRecorder::const_iterator GameRecorder::cend() const
{
	return _iterationRecords.cend();
}
