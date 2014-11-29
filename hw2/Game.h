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

class ShipState
{
public:
	Ship & _ship;
	std::vector<Item *> _piledObjects;
	Direction _shipDirection;


	ShipState(Ship & ship) : _ship(ship)
	{
	}
};

class Game
{
private:
	static const unsigned SMALL_SHIP_INDEX = 0;
	static const unsigned BIG_SHIP_INDEX = 1;
	static const unsigned SHIPS_COUNT = 2;

	struct GameObjects
	{
		std::vector<GameObject *> _all;
		std::vector<GameObject *> _blocking;

		std::vector<Item *> _items;
		std::vector<Wall *> _walls;

		BigShip & _bigShip;
		SmallShip & _smallShip;

		ExitPoint * _exitPoint;

		GameObjects(SmallShip & smallShip, BigShip & bigShip) : _smallShip(smallShip), _bigShip(bigShip)
		{
		}
	}_gameObjects;

	struct
	{
		bool _exit;
	}_state;

	struct
	{
		(ShipState *) _shipStates[Game::SHIPS_COUNT];

		bool _rotateSmallShip;
	}_updateArgs;

	Canvas _canvas;

	static bool isInPool(const GameObject & gameObject, const std::vector<GameObject *> & pool);
	static bool isBlockedByAny(const GameObject & gameObject, Direction from, const std::vector<GameObject *> & blockingObjects);
	static bool isBlockedByAny(const GameObject & gameObject, Direction from, const std::vector<GameObject *> & blockingObjects, const std::vector<GameObject *> & ignore);

	void moveItems(std::vector<Item *> & items, Direction direction);

	void setInitialState();
	void readUserInput();
	void processUserInput();
	void update();
	void draw();

	bool isGameOver();

	//The following functions are the various sections of the "update(...)" function's implementation
	void calculateChanges();
	void applyChanges(); //The actual moving/translation/advancment of the game

	void getPiledItems(const GameObject & gameObject, std::vector<Item *> & result) const;
	void pushPile(GameObject & gameObject, Direction direction, std::vector<GameObject *> & pileMembers, std::vector<GameObject *> & pushableMembers);
	void removeShip(Ship & ship);
public:
	Game(SmallShip & smallShip, BigShip & bigShip);
	~Game();

	void run();

	void addGameObject(Item * item);
	void addGameObject(Wall * wall);
	void addGameObject(ExitPoint * exit);
};

#endif