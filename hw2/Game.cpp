#include "Game.h"

#include <vector>
#include <string>
#include <iostream>
#include <fstream>

#include "FilesManager.h"
#include "ScreenManager.h"
#include "GameScreenBuilder.h"
#include "GameOverScreen.h"
#include "MenuScreen.h"

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

void Game::startLevel()
{
	Game::_currentLevelFileName = "";
	bool lastFileReached = false;

	while(Game::_currentLevelFileName.empty() && !lastFileReached)
	{
		//Get closest-level file
		{
			std::vector<std::string> fileNames = FilesManager::getFilesList(FilesManager::FILE_TYPE_LEVEL);

			std::vector<unsigned> ids;
			{
				for(std::vector<std::string>::iterator fileName = fileNames.begin();
					fileName != fileNames.end(); ++fileName)
				{
					unsigned id;
					if(FilesManager::getScreenId(*fileName, id))
					{
						ids.push_back(id);
					}
					else
					{
						fileName = fileNames.erase(fileName);
						--fileName;
					}
				}
			}

			if(ids.empty())
			{
				lastFileReached = true;
			}

			unsigned closestIdIndex = 0;
			bool found = false;
			for(unsigned i = 0; i < ids.size(); ++i)
			{
				if(ids[i] >= Game::_currentLevelId)
				{
					if(found == false)
					{
						found = true;
						closestIdIndex = i;
					}
					else if(ids[i] <= ids[closestIdIndex])
					{
						closestIdIndex = i;
					}
				}
			}

			if(found)
			{
				Game::_currentLevelId = ids[closestIdIndex];
				Game::_currentLevelFileName = fileNames[closestIdIndex];
			}
			else
			{
				++Game::_currentLevelId;
			}
		}

		if(lastFileReached)
		{
			MenuScreen * congratulationsScreen = new MenuScreen();
			congratulationsScreen->append("Congratulations!");
			congratulationsScreen->append("You may receive the \"Challanger\" bedge");
			congratulationsScreen->append(" by subscribing to our Facebook page!");

			//TODO: Append a high-score board

			ScreenManager::add(congratulationsScreen);
		}
		else
		{
			const std::string levelFile;

			GameScreenBuilder builder;
			builder.loadFromFile(Game::_currentLevelFileName);
			if(builder.isValid())
			{
				Game::_currentGameScreen = (GameScreen *)builder.build();
				ScreenManager::add(Game::_currentGameScreen);
			}
			else
			{
				Game::startNextLevel(); //Retry with another ID
			}
		}
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
	//TODO: Re-implement
	//Get save-file name
	std::cout << "\fPlease choose a name for this game-save and press <Enter>";
	std::string saveName;
	std::cin >> saveName;

	std::ofstream * saveFile = FilesManager::createSaveFile(saveName);

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
	std::ofstream * solutionFile = FilesManager::createSolutionFile(Game::_currentLevelFileName);

	*solutionFile << "ScreenID=" << Game::_currentLevelId << std::endl;

	std::cout << "\fPlease provide your name for the score-screen and press <Enter>";
	std::string solverName;
	std::cin >> solverName; //TODO: Re-implement
	*solutionFile << "NameOfSolver=" << solverName << std::endl;

	*solutionFile << Game::getSteps();

	solutionFile->close();
	delete solutionFile;
}

void Game::gameOver()
{
	ScreenManager::add(new GameOverScreen());
}

void Game::exit()
{
	ScreenManager::remove(Game::_currentGameScreen);
}
