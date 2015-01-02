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

		if(FilesManager::getScreenId(*fileName, FilesManager::FileType::FILE_TYPE_LEVEL, fileInfo.id))
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
		Game::displayEndGameScreen();
	}
}

void Game::start(const unsigned id)
{
	std::vector<std::string> levels = FilesManager::getFilesList(FilesManager::FILE_TYPE_LEVEL);
	if(levels.empty())
	{
		MenuScreen * noLevelsScreen = new MenuScreen();
		noLevelsScreen->append("Sorry, there are levels to be played.");
		ScreenManager::add(noLevelsScreen);
	}
	else
	{
		Game::_currentLevelId = id;
		Game::startLevel();
	}
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

void Game::loadGame(const std::string & saveFileName, unsigned screenId)
{
	Game::_currentLevelId = screenId;
	Game::startLevel();

	std::ifstream * saveFile = FilesManager::openFile(saveFileName, FilesManager::FILE_TYPE_SAVE);
	std::string line;

	std::getline(*saveFile, line); //Skip Screen ID

	std::getline(*saveFile, line); //Read iterations counter
	unsigned iterationsCounter;
	sscanf(line.c_str(), "ClockIterations=%u", &iterationsCounter);

	//Skip Screen description
	for(unsigned i = 0; i < Canvas::getHeight(); ++i)
	{
		std::getline(*saveFile, line);
	}

	Canvas canvas;

	//Load & apply iterations
	Screen & screen = *Game::_currentGameScreen;
	Keyboard dummyKeyboard;
	unsigned currentIteration = 0;
	while(std::getline(*saveFile, line))
	{
		unsigned iteration;
		char key1;
		char key2;

		const int parametersCount = sscanf(line.c_str(), "%u: %c%c", &iteration, &key1, &key2);

		while(currentIteration < iteration - 1)
		{
			screen.setInitialState();
			screen.readUserInput(dummyKeyboard);
			dummyKeyboard.update();
			screen.process();
			screen.update();

			canvas.begin();
			screen.draw(canvas);
			canvas.end();

			++currentIteration;
		}

		if(parametersCount >= 2)
		{
			dummyKeyboard.setPress(key1);
		}

		if(parametersCount >= 3)
		{
			dummyKeyboard.setPress(key2);
		}
	}

	do
	{
		screen.setInitialState();
		screen.readUserInput(dummyKeyboard);
		dummyKeyboard.update();
		screen.process();
		screen.update();

		++currentIteration;
	}
	while(iterationsCounter > currentIteration);

	saveFile->close();
	delete saveFile;
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

void Game::displayEndGameScreen()
{
	MenuScreen * congratulationsScreen = new MenuScreen();
	congratulationsScreen->append("Congratulations!");
	congratulationsScreen->append("You may receive the \"Challanger\" bedge");
	congratulationsScreen->append(" by subscribing to our Facebook page!");
	congratulationsScreen->append("");
	congratulationsScreen->append("");
	congratulationsScreen->append("Best scores:");

	std::vector<std::string> levels = FilesManager::getFilesList(FilesManager::FILE_TYPE_LEVEL);
	std::vector<std::string> solutions = FilesManager::getFilesList(FilesManager::FILE_TYPE_SOLUTION);
	for(unsigned i = 0; i < levels.size(); ++i)
	{
		std::string score = std::to_string(i + 1) + ". " + levels[i] + " - ";

		std::vector<std::string>::const_iterator bestSolution =
			std::find(solutions.cbegin(), solutions.cend(), levels[i]);
		if(bestSolution == solutions.cend())
		{
			score += "Unsolved!";
		}
		else
		{
			std::ifstream * solutionFile = FilesManager::openFile(levels[i], FilesManager::FILE_TYPE_SOLUTION);

			std::string line;
			std::getline(*solutionFile, line); //Skip screen ID
			std::getline(*solutionFile, line); //Get solver name
			score += "by ";
			score += &(line.c_str()[strlen("NameOfSolver=")]);

			//Get moves cout
			unsigned iterationsCount = 0;
			while(std::getline(*solutionFile, line))
			{
				unsigned iterationIndex;
				char colon;
				if(sscanf(line.c_str(), "%u%c", &iterationIndex, &colon) == 2 && colon == ':')
				{
					iterationsCount = iterationIndex + 1;
				}
			}

			score += " in " + std::to_string(iterationsCount) + " clock-ticks";

			solutionFile->close();
			delete solutionFile;
		}

		congratulationsScreen->append(score);
	}

	ScreenManager::add(congratulationsScreen);
}

void Game::gameOver()
{
	ScreenManager::add(new GameOverScreen());
}

void Game::exit()
{
	ScreenManager::remove(Game::_currentGameScreen);
}
