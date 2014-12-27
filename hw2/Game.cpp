#include "Game.h"

#include <vector>
#include <string>

#include "GameConfig.h"
#include "FilesManager.h"
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
		std::vector<std::string> fileNames = FilesManager::getFileNames();

		//Get ID per SPG file
		std::vector<unsigned> ids;
		{
			for(unsigned i = 0; i < fileNames.size(); ++i)
			{
				const std::string filePath = GameConfig::getLevelsPath() +  fileNames[i];

				unsigned id;
				if(FilesManager::getScreenId(filePath, id))
				{
					ids.push_back(id);
				}
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
		const std::string levelFile = GameConfig::getLevelsPath() + nextLevelFileName;

		GameScreenBuilder builder;
		builder.loadFromFile(levelFile);
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
