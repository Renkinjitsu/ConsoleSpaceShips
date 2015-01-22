#ifndef GAME_SCREEN_BUILDER_H_
#define GAME_SCREEN_BUILDER_H_

#include <vector>
#include <string>

#include "Screen.h"

#include "Wall.h"
#include "Item.h"
#include "Bomb.h"
#include "BadShip.h"
#include "BigShip.h"
#include "SmallShip.h"
#include "ExitPoint.h"

#include "GameConfig.h"

class GameScreenBuilder
{
private:
	std::vector<Wall *> _walls;
	std::vector<Item *> _items;
	std::vector<Bomb *> _bombs;
	std::vector<BadShip *> _badShips;
	std::vector<BigShip *> _bigShips;
	std::vector<SmallShip *> _smallShips;
	std::vector<ExitPoint *> _exitPoints;

	std::vector<std::string> _errors;

	static bool isValidCharacter(char character);
	static void floodFill4Way(char * const serializedCanvas, const char targetCharacter, const Point & coordinate, std::vector<Point> & points);

	void clear();

public:
	GameScreenBuilder();
	~GameScreenBuilder();

	void loadFromFile(const std::string & fileName);

	bool isValid() const;

	Screen * build();
};

#endif /* GAME_SCREEN_BUILDER_H_ */
