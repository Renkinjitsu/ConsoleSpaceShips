#ifndef _GAME_H_
#define _GAME_H_

#include <vector>

#include "Direction.h"
#include "Item.h"
#include "BigShip.h"
#include "SmallShip.h"
#include "Wall.h"
#include "ExitPoint.h"
#include "Canvas.h"

class Game
{
private:
	std::vector<GameObject *> _allGameObjects;
	std::vector<GameObject *> _allBlockingObjects;

	std::vector<Item *> _items;
	std::vector<Wall *> _walls;

	BigShip * _bigShip;
	SmallShip * _smallShip;
	unsigned _presentShipsCount;

	ExitPoint * _exitPoint;

	Direction _bigShipDirection;
	Direction _smallShipDirection;
	bool _smallShipTryRotate;

	Canvas _canvas;

	bool _exit;

	static bool isBlockedByAny(const GameObject & gameObject, Direction from, const std::vector<GameObject *> & blockingObjects);

	void moveItems(std::vector<Item *> & items, Direction direction);

	void readUserInput();
	void update();
	void draw_all();

	bool isGameOver();

	void getPiledItems(const GameObject & gameObjectm, std::vector<Item *> & result) const;
public:
	Game();

	void run();

	void addGameObject(Item * item);
	void addGameObject(BigShip * bigShip);
	void addGameObject(SmallShip * smallShip);
	void addGameObject(Wall * wall);
	void addGameObject(ExitPoint * exit);
};

#endif