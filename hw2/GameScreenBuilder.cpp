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
			const Point nextCoordinate = coordinate + Point::DIRECTIONS[i];

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

void GameScreenBuilder::loadFromFile(const std::string & fileName)
{
	this->clear();

	std::ifstream * levelFile = FilesManager::openFile(fileName, FilesManager::FILE_TYPE_LEVEL);
	std::string line;

	//Screen ID
	if(levelFile->eof())
	{
		_errors.push_back("No screen ID");
	}
	else
	{
		std::getline(*levelFile, line);
		unsigned id;
		if(sscanf(line.c_str(), "ScreenID=%u", &id) != 1)
		{
			_errors.push_back("Corrupted screen ID line");
		}
		else
		{
			//Check for duplicate ID
			const std::vector<std::string> & fileNames = FilesManager::getFilesList(FilesManager::FILE_TYPE_LEVEL);
			for(unsigned i = 0; i < fileNames.size(); ++i)
			{
				if(fileNames[i] != fileName)
				{
					unsigned otherId;
					if(FilesManager::getScreenId(fileNames[i], FilesManager::FileType::FILE_TYPE_LEVEL, otherId))
					{
						if(id == otherId)
						{
							_errors.push_back("Duplicate ID - \"" + fileNames[i] + "\"");
						}
					}
				}
			}
		}
	}

	//Load serialized canvas
	char serializedCanvas[Canvas::MAX_SERIALIZED_LENGTH];
	unsigned serializedPosition = 0;
	for(unsigned lineIndex = 0; lineIndex < Canvas::getHeight(); ++lineIndex)
	{
		if(levelFile->eof())
		{
			line = "";
		}
		else
		{
			std::getline(*levelFile, line);
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

	levelFile->close();
	delete levelFile;

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
				for(unsigned i = 0; i < points.size(); ++i)
				{
					_walls.push_back(new Wall(points[i]));
				}
			}
			break;

			case GameConfig::TEXTURES_EXIT:
			{
				_exitPoints.push_back(new ExitPoint(points));
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

					_smallShips.push_back(new SmallShip(bottomLeft, isHorisontal));
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

					const Point bottomLeft(left, bottom);
					const Point topRight = Point(right, top) + Point::DOWN + Point::LEFT;

					if(bottomLeft == topRight)
					{
						_bigShips.push_back(new BigShip(bottomLeft));
					}
					else
					{
						error += "Invalid big spaceship";
					}
				}
			}
			break;

			case GameConfig::TEXTURES_BAD_SPACESHIP:
			{
				for(unsigned i = 0; i < points.size(); ++i)
				{
					_badShips.push_back(new BadShip(points[i]));
				}
			}
			break;

			case GameConfig::TEXTURES_BOMB:
			{
				for(unsigned i = 0; i < points.size(); ++i)
				{
					_bombs.push_back(new Bomb(points[i]));
				}
			}
			break;

			default: //ch is in [1 .. 9]
			{
				bool isDuplicateItem = false;
				for(unsigned i = 0; i < _items.size(); ++i)
				{
					isDuplicateItem |= (_items[i]->getTexture() == character);
				}

				if(isDuplicateItem)
				{
					error += "Duplicated item \'";
					error += character;
					error += '\'';
				}
				else
				{
					_items.push_back(new Item(character, points));
				}
			}
			break;
		}

		if(error.length() > 0)
		{
			const unsigned lineOffset = 2; //First line is marked as '1', & is ocupied by the screen ID
			const unsigned columnOffset = 1; //First column is marked as '1'

			const unsigned lineIndex = serializedPosition / Canvas::getWidth();
			const unsigned characterIndex = serializedPosition % Canvas::getWidth();
			error += " at line " + std::to_string(lineOffset + lineIndex);
			error += ", character " + std::to_string(columnOffset + characterIndex);
			_errors.push_back(error);
		}
	}

	//Count validations
	if(noSmallSpaceship)
	{
		_errors.push_back("No small spaceship");
	}
	else if(_smallShips.size() > 1)
	{
		_errors.push_back("Too many small spaceships");
	}

	if(noBigSpaceship)
	{
		_errors.push_back("No big spaceship");
	}
	else if(_bigShips.size() > 1)
	{
		_errors.push_back("Too many big spaceships");
	}

	if(_exitPoints.size() == 0)
	{
		_errors.push_back("No exit point");
	}
}

bool GameScreenBuilder::isValid() const
{
	return _errors.empty();
}

Screen * GameScreenBuilder::build()
{
	Screen * screen = NULL;

	if(this->isValid())
	{
		GameScreen * game = new GameScreen();
		screen = game;

		for(unsigned i = 0; i < _items.size(); ++i)
		{
			game->addGameObject(_items[i]);
		}
		_items.clear();

		for(unsigned i = 0; i < _exitPoints.size(); ++i)
		{
			game->addGameObject(_exitPoints[i]);
		}
		_exitPoints.clear();

		for(unsigned i = 0; i < _walls.size(); ++i)
		{
			game->addGameObject(_walls[i]);
		}
		_walls.clear();

		for(unsigned i = 0; i < _smallShips.size(); ++i)
		{
			game->addGameObject(_smallShips[i]);
		}
		_smallShips.clear();

		for(unsigned i = 0; i < _bigShips.size(); ++i)
		{
			game->addGameObject(_bigShips[i]);
		}
		_bigShips.clear();

		for(unsigned i = 0; i < _badShips.size(); ++i)
		{
			game->addGameObject(_badShips[i]);
		}
		_badShips.clear();

		for(unsigned i = 0; i < _bombs.size(); ++i)
		{
			game->addGameObject(_bombs[i]);
		}
		_bombs.clear();
	}
	else
	{
		MenuScreen * errors = new MenuScreen();
		screen = errors;

		errors->append("Errors:");
		for(unsigned i = 0; i < _errors.size(); ++i)
		{
			errors->append(std::to_string(i + 1) + ". " + _errors[i]);
		}
	}

	this->clear();

	return screen;
}

void GameScreenBuilder::clear()
{
	for(unsigned i = 0; i < _items.size(); ++i)
	{
		delete _items[i];
	}
	_items.clear();

	for(unsigned i = 0; i < _exitPoints.size(); ++i)
	{
		delete _exitPoints[i];
	}
	_exitPoints.clear();

	for(unsigned i = 0; i < _walls.size(); ++i)
	{
		delete _walls[i];
	}
	_walls.clear();

	for(unsigned i = 0; i < _smallShips.size(); ++i)
	{
		delete _smallShips[i];
	}
	_smallShips.clear();

	for(unsigned i = 0; i < _bigShips.size(); ++i)
	{
		delete _bigShips[i];
	}
	_bigShips.clear();

	_errors.clear();
}
