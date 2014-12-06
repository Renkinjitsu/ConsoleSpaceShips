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
	Direction _shipDirection;

	std::vector<Item *> _crashPotentialItems;
	std::vector<GameObject *> _pushPile; //A pile that the ship is currently tring to push

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
		ShipState * _shipStates[Game::SHIPS_COUNT];

		bool _rotateSmallShip;
		std::vector<Item *> _freeFallingItems;
	}_updateArgs;

	Canvas & _canvas;

	static bool isEqual(const std::vector<GameObject *> & a, const std::vector<GameObject *> & b);
	static bool isInPool(const GameObject & gameObject, const std::vector<GameObject *> & pool);
	static bool isInPool(const Item & item, const std::vector<Item *> & pool);
	static bool isBlockedByAny(const GameObject & gameObject, Direction from, const std::vector<GameObject *> & blockingObjects);
	static bool isBlockedByAny(const GameObject & gameObject, Direction from, const std::vector<GameObject *> & blockingObjects, const std::vector<GameObject *> & ignore);

	void moveItems(std::vector<Item *> & items, Direction direction);

	void listCrashPotentialItems(ShipState & shipState);
	void listFreeFallingItems();
	void refineCrashPotentialItems(ShipState & shipState);
	void expandCrashPotentialItems(ShipState & shipState);
	unsigned getTotalMass(const std::vector<GameObject *> & pool) const;

	void setInitialState();
	void processUserInput();
	void update();
	void draw();

	void gameOverScreen();

	bool isGameOver();

	void readUserInput();
	void applyChanges(); //The actual moving/translation/advancment of the game

	void getPiledItems(const Item & gameObject, std::vector<Item *> & result) const;
	void getPushPile(GameObject & currentPileElement, Direction direction, std::vector<GameObject *> & pileMembers) const;
	void removeShip(Ship & ship);
public:
	Game(Canvas & _canvas, SmallShip & smallShip, BigShip & bigShip);
	~Game();

	void run();

	void addGameObject(Item * item);
	void addGameObject(Wall * wall);
	void addGameObject(ExitPoint * exit);
};

#endif