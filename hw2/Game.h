#ifndef _GAME_H_
#define _GAME_H_

#include "Canvas.h"

#include "GameObjectSet.h"

#include "Direction.h"
#include "Item.h"
#include "BigShip.h"
#include "SmallShip.h"
#include "Wall.h"
#include "ExitPoint.h"

class ShipState
{
public:
	Ship & _ship;
	Direction _direction;

	GameObjectSet _crashPotentialItems;
	GameObjectSet _pushPile; //A pile that the ship is currently tring to push

	ShipState(Ship & ship) : _ship(ship)
	{
		//Do nothing
	}
};

class Game
{
private:
	static const unsigned SMALL_SHIP_INDEX = 0;
	static const unsigned BIG_SHIP_INDEX = 1;
	static const unsigned SHIPS_COUNT = 2;

	GameObjectSet _allGameObjects;
	GameObjectSet _blockingGameObjects;
	GameObjectSet _items;
	GameObject * _exitPoint;
	BigShip & _bigShip;
	SmallShip & _smallShip;

	struct
	{
		bool _exit;
	}_state;

	struct
	{
		ShipState * _shipStates[Game::SHIPS_COUNT];

		bool _rotateSmallShip;
		GameObjectSet _freeFallingItems;
	}_updateArgs;

	Canvas & _canvas;

	static bool isBlockedByAny(const GameObject & gameObject, Direction from, const GameObjectSet & blockingObjects);
	static bool isBlockedByAny(const GameObject & gameObject, Direction from, const GameObjectSet & blockingObjects, const GameObjectSet & ignore);

	void getPiledItems(const GameObject & gameObject, GameObjectSet & result) const;
	void getPushPile(GameObject & currentPileElement, Direction direction, GameObjectSet & pileMembers) const;

	void removeShip(Ship & ship);
	void moveItems(GameObjectSet & items, Direction direction);

	void listCrashPotentialItems(ShipState & shipState);
	void listFreeFallingItems();
	void refineCrashPotentialItems(ShipState & shipState); //Remove items that are not crashing into the ship
	void expandCrashPotentialItems(ShipState & shipState);
	unsigned getTotalMass(const GameObjectSet & gameObjects) const;

	void setInitialState();
	void processUserInput();
	void update();
	void draw();

	void gameOverScreen();

	bool isGameOver() const;

	void readUserInput();
	void applyChanges(); //The actual moving/translation/advancment of the game

public:
	Game(Canvas & _canvas, SmallShip & smallShip, BigShip & bigShip);
	~Game();

	void run();

	void addGameObject(Item * item);
	void addGameObject(Wall * wall);
	void addGameObject(ExitPoint * exit);
};

#endif