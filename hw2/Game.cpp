#include "Game.h"

#include <vector>
#include <string>
#include <iostream>
#include <fstream>

#include "ScreenManager.h"
#include "GameScreenBuilder.h"
#include "GameOverScreen.h"
#include "GameScreen.h"

unsigned Game::_nextLevelId;

void Game::startLevel()
{
	std::string nextLevelFileName("");
	bool found = false;

	//Get closest-level file
	{
		//Get file-names-list
		std::vector<std::string> fileNames;
		{
			std::string spgs("spgList");
			std::string command("dir /b ");
			command += "C:\\Users\\Snow\\Source\\Repos\\ConsoleSpaceShips\\hw2";
			command += "\\*.spg > ";
			command += spgs;

			system(command.c_str());

			std::ifstream spgsFile(spgs);
			std::string str;
			while(std::getline(spgsFile, str))
			{
				fileNames.push_back(str);
			}
			spgsFile.close();
		}

		//Get ID per SPG file
		std::vector<unsigned> ids;
		{
			for(unsigned i = 0; i < fileNames.size(); ++i)
			{
				std::ifstream levelFile(fileNames[i]);
				std::string idLine;
				std::getline(levelFile, idLine);
				levelFile.close();

				unsigned id;
				sscanf(idLine.c_str(), "ScreenID = %u", &id);

				ids.push_back(id);
			}
		}

		unsigned closestIdIndex = 0;
		for(unsigned i = 0; i < ids.size(); ++i)
		{
			if(ids[i] >= Game::_nextLevelId)
			{
				if(found == false)
				{
					found = true;
					closestIdIndex = i;
				}
				else if(ids[i] < ids[closestIdIndex])
				{
					closestIdIndex = i;
				}
			}
		}

		if(found)
		{
			Game::_nextLevelId = ids[closestIdIndex];
			nextLevelFileName = fileNames[closestIdIndex];
		}
	}

	if(found)
	{
		GameScreenBuilder builder;
		builder.loadFromFile(nextLevelFileName);
		//builder.loadTestLevel();
		ScreenManager::add(builder.build());
	}
	else
	{
		//Display "CongratulationsScreen" (followed by high-score screen)
	}
}

void Game::start()
{
	Game::_nextLevelId = 0;

	Game::startLevel();
}

void Game::startNextLevel()
{
	++Game::_nextLevelId;

	Game::startLevel();
}

void Game::gameOver()
{
	ScreenManager::add(new GameOverScreen());
}
