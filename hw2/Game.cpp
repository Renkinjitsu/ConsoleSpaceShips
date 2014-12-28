#include "Game.h"

#include <vector>
#include <string>
#include <iostream>
#include <fstream>

#include "GameConfig.h"
#include "FilesManager.h"
#include "ScreenManager.h"
#include "GameScreenBuilder.h"
#include "GameOverScreen.h"
#include "MenuScreen.h"

unsigned Game::_currentLevelId;
GameScreen * Game::_currentGameScreen;
std::string Game::_currentLevelFileName("");

void Game::startLevel()
{
	Game::_currentLevelFileName = "";
	bool lastFileReached = false;

	while(Game::_currentLevelFileName.empty() && !lastFileReached)
	{
		//Get closest-level file
		{
			std::vector<std::string> fileNames = FilesManager::getFileNames();

			std::vector<unsigned> ids;
			{
				for(std::vector<std::string>::iterator fileName = fileNames.begin();
					fileName != fileNames.end(); ++fileName)
				{
					const std::string filePath = GameConfig::getLevelsPath() + *fileName;

					unsigned id;
					if(FilesManager::getScreenId(filePath, id))
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
			const std::string levelFile = GameConfig::getLevelsPath() + Game::_currentLevelFileName;

			GameScreenBuilder builder;
			builder.loadFromFile(levelFile);
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

void Game::start()
{
	Game::_currentLevelId = 0;

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
	std::string stringScreenId = std::to_string(Game::_currentLevelId);
	const std::string filePath = GameConfig::getLevelsPath() + stringScreenId + GameConfig::FILE_EXTENSION_GAME_SAVE;
	std::ofstream saveFile(filePath, std::ios::out | std::ios::trunc);

	saveFile << "ScreenID=" << stringScreenId << std::endl;

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
			saveFile << *canvasCharacter;
			++canvasCharacter;
		}

		saveFile << std::endl;
	}

	saveFile.close();
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
	//TODO: Implement
}

void Game::gameOver()
{
	ScreenManager::add(new GameOverScreen());
}

void Game::exit()
{
	ScreenManager::remove(Game::_currentGameScreen);
}
