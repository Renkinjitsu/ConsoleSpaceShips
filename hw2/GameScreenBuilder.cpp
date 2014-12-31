#include "GameScreenBuilder.h"

#include <iostream>
#include <fstream>
#include <string.h>

#include "GameConfig.h"
#include "GameScreen.h"
#include "MenuScreen.h"
#include "Point.h"
#include "FilesManager.h"

bool GameScreenBuilder::isValidCharacter(char character)
{
	bool isValid = false;

	for(unsigned i = 0; i < GameConfig::allowedTexturesCount; ++i)
	{
		isValid |= (GameConfig::allowedTextures[i] == character);
	}

	return isValid;
}

void GameScreenBuilder::floodFill4Way(char * const serializedCanvas, const char targetCharacter, const Point & coordinate, std::vector<Point> & points)
{
	char & currentCharacter = serializedCanvas[Canvas::serialize(coordinate)];

	if(currentCharacter == targetCharacter)
	{
		currentCharacter = GameConfig::TEXTURES_EMPTY; //Replace with a "neutral" symbol, so it will be ignored next time it is visited

		points.push_back(coordinate);

		for(unsigned i = 0; i < Point::DIRECTIONS_COUNT; ++i)
		{
			Point nextCoordinate = coordinate;
			nextCoordinate.move(Point::DIRECTIONS[i]);

			GameScreenBuilder::floodFill4Way(serializedCanvas, targetCharacter, nextCoordinate, points);
		}
	}
}

GameScreenBuilder::GameScreenBuilder()
{
}

GameScreenBuilder::~GameScreenBuilder()
{
	this->clear();
}

void GameScreenBuilder::loadFromFile(const std::string & filePath)
{
	this->clear();

	std::ifstream levelFile(filePath);
	std::string line;

	//ID
	if(levelFile.eof())
	{
		this->_errors.push_back("No screen ID");
	}
	else
	{
		std::getline(levelFile, line);
		unsigned id;
		if(sscanf(line.c_str(), "ScreenID=%u", &id) != 1)
		{
			this->_errors.push_back("Corrupted screen ID line");
		}
		else
		{
			const std::vector<std::string> & fileNames = FilesManager::getFilesList(FilesManager::FILE_TYPE_LEVEL);
			for(unsigned i = 0; i < fileNames.size(); ++i)
			{
				std::string path = fileNames[i];
				if(path != filePath)
				{
					unsigned otherId;
					if(FilesManager::getScreenId(path, otherId))
					{
						if(id == otherId)
						{
							this->_errors.push_back("Duplicate ID - \"" + path + "\"");
						}
					}
				}
			}
		}
	}

	const unsigned lineOffset = 2; //First line is marked as '1', & is ocupied by the screen ID
	const unsigned columnOffset = 1; //First column is marked as '1'

	//Load serialized canvas
	char serializedCanvas[Canvas::MAX_SERIALIZED_LENGTH];
	unsigned serializedPosition = 0;
	for(unsigned lineIndex = 0; lineIndex < Canvas::getHeight(); ++lineIndex)
	{
		if(levelFile.eof())
		{
			line = "";
		}
		else
		{
			std::getline(levelFile, line);
		}

		for(unsigned i = 0; i < Canvas::getWidth(); ++i, ++serializedPosition)
		{
			const char character = (i < line.length()) ? line.c_str()[i] : GameConfig::TEXTURES_EMPTY;

			if(GameScreenBuilder::isValidCharacter(character))
			{
				serializedCanvas[serializedPosition] = character;
			}
			else
			{
				serializedCanvas[serializedPosition] = GameConfig::TEXTURES_WALL; //As instructed in the requirements documentation
			}
		}
	}

	levelFile.close();

	bool noBigSpaceship = true;
	bool noSmallSpaceship = true;

	//Recognize items
	for(unsigned serializedPosition = 0; serializedPosition < Canvas::MAX_SERIALIZED_LENGTH; ++serializedPosition)
	{
		const char character = serializedCanvas[serializedPosition];
		if(character == GameConfig::TEXTURES_EMPTY)
		{
			continue; //Skip
		}

		std::vector<Point> points;
		const Point & coordinate = Canvas::deserialize(serializedPosition);
		GameScreenBuilder::floodFill4Way(serializedCanvas, character, coordinate, points);

		std::string error;

		switch(character)
		{
			case GameConfig::TEXTURES_WALL:
			{
				this->_walls.push_back(new Wall(points));
			}
			break;

			case GameConfig::TEXTURES_EXIT:
			{
				this->_exitPoints.push_back(new ExitPoint(points));
			}
			break;

			case GameConfig::TEXTURES_SMALL_SPACESHIP:
			{
				noSmallSpaceship = false;

				if(points.size() != 2)
				{
					error += "Invalid small spaceship";
				}
				else
				{
					const Point bottomLeft(Point::getLeft(points[0], points[1]), Point::getBottom(points[0], points[1]));
					const bool isHorisontal = (points[0].getY() == points[1].getY());

					this->_smallShips.push_back(new SmallShip(bottomLeft, isHorisontal));
				}
			}
			break;

			case GameConfig::TEXTURES_BIG_SPACESHIP:
			{
				noBigSpaceship = false;

				if(points.size() != 4)
				{
					error += "Invalid big spaceship";
				}
				else
				{
					const unsigned top = Point::getTop(Point::getTop(points[0], points[1]), Point::getTop(points[2], points[3]));
					const unsigned bottom = Point::getBottom(Point::getBottom(points[0], points[1]), Point::getBottom(points[2], points[3]));
					const unsigned left = Point::getLeft(Point::getLeft(points[0], points[1]), Point::getLeft(points[2], points[3]));
					const unsigned right = Point::getRight(Point::getRight(points[0], points[1]), Point::getRight(points[2], points[3]));

					Point bottomLeft(left, bottom);
					Point topRight(right, top);

					topRight.move(Point::DOWN);
					topRight.move(Point::LEFT);

					if(bottomLeft.equals(topRight))
					{
						this->_bigShips.push_back(new BigShip(bottomLeft));
					}
					else
					{
						error += "Invalid big spaceship";
					}
				}
			}
			break;

			default: //ch is in [1 .. 9]
			{
				bool isDuplicateItem = false;
				for(unsigned i = 0; i < this->_items.size(); ++i)
				{
					isDuplicateItem |= (this->_items[i]->getTexture() == character);
				}

				if(isDuplicateItem)
				{
					error += "Duplicated item \'";
					error += character;
					error += '\'';
				}
				else
				{
					this->_items.push_back(new Item(character, points));
				}
			}
			break;
		}

		if(error.length() > 0)
		{
			const unsigned lineIndex = serializedPosition / Canvas::getWidth();
			const unsigned characterIndex = serializedPosition % Canvas::getWidth();
			error += " at line " + std::to_string(lineOffset + lineIndex);
			error += ", character " + std::to_string(columnOffset + characterIndex);
			this->_errors.push_back(error);
		}
	}

	//Count validations
	if(noSmallSpaceship)
	{
		this->_errors.push_back("No small spaceship");
	}
	else if(this->_smallShips.size() > 1)
	{
		this->_errors.push_back("Too many small spaceships");
	}

	if(noBigSpaceship)
	{
		this->_errors.push_back("No big spaceship");
	}
	else if(this->_bigShips.size() > 1)
	{
		this->_errors.push_back("Too many big spaceships");
	}

	if(this->_exitPoints.size() == 0)
	{
		this->_errors.push_back("No exit point");
	}
}

bool GameScreenBuilder::isValid() const
{
	return this->_errors.empty();
}

Screen * GameScreenBuilder::build()
{
	Screen * screen = NULL;

	if(this->isValid())
	{
		GameScreen * game = new GameScreen();
		screen = game;

		for(unsigned i = 0; i < this->_items.size(); ++i)
		{
			game->addGameObject(this->_items[i]);
		}
		this->_items.clear();

		for(unsigned i = 0; i < this->_exitPoints.size(); ++i)
		{
			game->addGameObject(this->_exitPoints[i]);
		}
		this->_exitPoints.clear();

		for(unsigned i = 0; i < this->_walls.size(); ++i)
		{
			game->addGameObject(this->_walls[i]);
		}
		this->_walls.clear();

		for(unsigned i = 0; i < this->_smallShips.size(); ++i)
		{
			game->addGameObject(this->_smallShips[i]);
		}
		this->_smallShips.clear();

		for(unsigned i = 0; i < this->_bigShips.size(); ++i)
		{
			game->addGameObject(this->_bigShips[i]);
		}
		this->_bigShips.clear();
	}
	else
	{
		MenuScreen * errors = new MenuScreen();
		screen = errors;

		errors->append("Errors:");
		for(unsigned i = 0; i < this->_errors.size(); ++i)
		{
			errors->append(std::to_string(i + 1) + ". " + this->_errors[i]);
		}
	}

	this->clear();

	return screen;
}

void GameScreenBuilder::clear()
{
	for(unsigned i = 0; i < this->_items.size(); ++i)
	{
		delete this->_items[i];
	}
	this->_items.clear();

	for(unsigned i = 0; i < this->_exitPoints.size(); ++i)
	{
		delete this->_exitPoints[i];
	}
	this->_exitPoints.clear();

	for(unsigned i = 0; i < this->_walls.size(); ++i)
	{
		delete this->_walls[i];
	}
	this->_walls.clear();

	for(unsigned i = 0; i < this->_smallShips.size(); ++i)
	{
		delete this->_smallShips[i];
	}
	this->_smallShips.clear();

	for(unsigned i = 0; i < this->_bigShips.size(); ++i)
	{
		delete this->_bigShips[i];
	}
	this->_bigShips.clear();

	this->_errors.clear();
}
