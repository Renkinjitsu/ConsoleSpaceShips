#ifndef GAME_SCREEN_BUILDER_H_
#define GAME_SCREEN_BUILDER_H_

#include <vector>
#include <string>

#include "Screen.h"

#include "Wall.h"
#include "Item.h"
#include "BigShip.h"
#include "SmallShip.h"
#include "ExitPoint.h"

#define GAME_HEIGHT 24
#define GAME_WIDTH 80

class GameScreenBuilder
{
private:
	static const char * const _validGameCaracters;

	std::vector<Wall *> _walls;
	std::vector<Item *> _items;
	std::vector<BigShip *> _bigShips;
	std::vector<SmallShip *> _smallShips;
	std::vector<ExitPoint *> _exitPoints;

	std::vector<std::string> _errors;
	bool _isCleared;

	static bool isValidCharacter(char character);
	static void floodFill4Way(char board[GAME_HEIGHT][GAME_WIDTH], char character, unsigned x, unsigned y, std::vector<Point> & points);

	void clear();

public:
	GameScreenBuilder();
	~GameScreenBuilder();

	void loadTestLevel();
	void loadFromFile(const std::string & filePath);

	bool isValid() const;

	Screen * build();
};

#endif /* GAME_SCREEN_BUILDER_H_ */
