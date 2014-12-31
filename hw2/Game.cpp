#include "Game.h"

#include <vector>
#include <iostream>
#include <fstream>

#include "FilesManager.h"
#include "ScreenManager.h"
#include "GameScreenBuilder.h"
#include "GameOverScreen.h"
#include "MenuScreen.h"

typedef struct
{
	const std::string * name;
	unsigned id;
}FileInfo;

unsigned Game::_currentLevelId;
GameScreen * Game::_currentGameScreen;
std::string Game::_currentLevelFileName("");

std::string Game::getSteps()
{
	std::string steps = "";

	const GameRecorder & gameRecorder = Game::_currentGameScreen->getRecorder();

	for(GameRecorder::const_iterator iteration = gameRecorder.cbegin();
		iteration != gameRecorder.cend(); ++iteration)
	{
		steps += std::to_string((*iteration).getId()) + ": ";
		for(GameIterationRecorder::const_iterator step = (*iteration).cbegin();
			step != (*iteration).cend(); ++step)
		{
			steps += *step;
		}
		steps += '\n';
	}

	return steps;
}

unsigned Game::getSolutionInterationsCount(const std::string & levelName)
{
	unsigned iterationsCount = 0;

	std::ifstream * solutionFile = FilesManager::openFile(levelName, FilesManager::FILE_TYPE_SOLUTION);

	std::string line;
	while(std::getline(*solutionFile, line))
	{
		unsigned iterationIndex;
		char colon;
		if(sscanf(line.c_str(), "%u%c", &iterationIndex, &colon) == 2 && colon == ':')
		{
			iterationsCount = iterationIndex + 1;
		}
	}

	solutionFile->close();
	delete solutionFile;

	return iterationsCount;
}

std::string Game::prompt(const std::string & message)
{
	std::string reply;

	system("cls");
	std::cout << message << " ";
	std::cin >> reply;
	system("cls");

	return reply;
}

void Game::startLevel()
{
	std::vector<std::string> fileNames = FilesManager::getFilesList(FilesManager::FILE_TYPE_LEVEL);

	//Get the ID of each level
	std::vector<FileInfo> files;
	for(std::vector<std::string>::iterator fileName = fileNames.begin();
		fileName != fileNames.end(); ++fileName)
	{
		FileInfo fileInfo;

		if(FilesManager::getScreenId(*fileName, fileInfo.id))
		{
			fileInfo.name = &(*fileName);
			files.push_back(fileInfo);
		}
		else
		{
			fileName = fileNames.erase(fileName);
			--fileName;
		}
	}

	//Sort
	for(unsigned i = 1; i < files.size(); ++i)
	{
		for(unsigned j = i; j > 0; --j)
		{
			if(files[j].id < files[j - 1].id)
			{
				std::swap(files[j].id, files[j - 1].id);
				std::swap(files[j].name, files[j - 1].name);
			}
		}
	}

	bool levelFound = false;

	//Get closest-level file
	for(unsigned i = 0; i < files.size() && !levelFound; ++i)
	{
		if(files[i].id >= Game::_currentLevelId)
		{
			Game::_currentLevelId = files[i].id;
			Game::_currentLevelFileName = *files[i].name;

			GameScreenBuilder builder;
			builder.loadFromFile(Game::_currentLevelFileName);
			if(builder.isValid())
			{
				Game::_currentGameScreen = (GameScreen *)builder.build();
				ScreenManager::add(Game::_currentGameScreen);

				levelFound = true;
			}
			else //Retry with a higher level ID
			{
				++Game::_currentLevelId;
			}
		}
	}

	if(!levelFound)
	{
		MenuScreen * congratulationsScreen = new MenuScreen();
		congratulationsScreen->append("Congratulations!");
		congratulationsScreen->append("You may receive the \"Challanger\" bedge");
		congratulationsScreen->append(" by subscribing to our Facebook page!");

		//TODO: Append a high-score board

		ScreenManager::add(congratulationsScreen);
	}
}

void Game::start(const unsigned id)
{
	Game::_currentLevelId = id;
	Game::startLevel();
}

void Game::startNextLevel()
{
	Game::saveSolution();

	++Game::_currentLevelId;
	Game::startLevel();
}

void Game::saveGame()
{
	//Get save-file name
	std::string saveName = Game::prompt("Game-save name:");

	std::ofstream * saveFile = FilesManager::createFile(saveName, FilesManager::FILE_TYPE_SAVE);

	*saveFile << "ScreenID=" << Game::_currentLevelId << std::endl;
	*saveFile << "ClockIterations=" << Game::_currentGameScreen->getIterations() << std::endl;

	Canvas canvas;
	canvas.begin();
	Game::_currentGameScreen->draw(canvas);
	std::string stringCanvas;
	canvas.end(stringCanvas);

	const char * canvasCharacter = stringCanvas.c_str();
	for(unsigned i = 0; i < Canvas::getHeight(); ++i)
	{
		for(unsigned j = 0; j < Canvas::getWidth(); ++j)
		{
			*saveFile << *canvasCharacter;
			++canvasCharacter;
		}

		*saveFile << std::endl;
	}

	*saveFile << Game::getSteps();

	saveFile->close();
	delete saveFile;
}

void Game::loadGame()
{
	//TODO: Implement
}

void Game::restart()
{
	ScreenManager::remove(Game::_currentGameScreen);
	const std::string & levelFilePath = Game::_currentLevelFileName;
	GameScreenBuilder builder;
	builder.loadFromFile(levelFilePath);
	Game::_currentGameScreen = (GameScreen *)builder.build();
	ScreenManager::add(Game::_currentGameScreen);
}

void Game::saveSolution()
{
	const unsigned currentGameIterations = Game::_currentGameScreen->getIterations();
	const unsigned currentSolutionIterations = Game::getSolutionInterationsCount(Game::_currentLevelFileName);
	if(currentSolutionIterations == 0 ||
		currentGameIterations < currentSolutionIterations)
	{
		std::string solverName = Game::prompt("Congratulations! You made a new record! What is your name?");

		std::ofstream * solutionFile = FilesManager::createFile(Game::_currentLevelFileName, FilesManager::FILE_TYPE_SOLUTION);
		std::string steps = Game::getSteps();

		*solutionFile << "ScreenID=" << Game::_currentLevelId << std::endl;
		*solutionFile << "NameOfSolver=" << solverName << std::endl;
		*solutionFile << steps;

		solutionFile->close();
		delete solutionFile;
	}
}

void Game::gameOver()
{
	ScreenManager::add(new GameOverScreen());
}

void Game::exit()
{
	ScreenManager::remove(Game::_currentGameScreen);
}
