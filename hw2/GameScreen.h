#ifndef GAME_SCREEN_H_
#define GAME_SCREEN_H_

#include "Screen.h"

#include "GameObjectSet.h"

#include "Item.h"
#include "BigShip.h"
#include "SmallShip.h"
#include "Wall.h"
#include "ExitPoint.h"
#include "GameRecorder.h"

//For inner usage only
struct ShipInfo
{
	Point _velocity;
	GameObjectSet _pushPile; //A pile that the ship is currently tring to push
	bool _rotate;
};

class GameScreen : public Screen
{
private:
	enum GameState
	{
		GAME_STATE_ONGOING,
		GAME_STATE_WON,
		GAME_STATE_LOST,
		GAME_STATE_QUIT,
	};

	static const unsigned SMALL_SHIP_INDEX = 0;
	static const unsigned BIG_SHIP_INDEX = 1;
	static const unsigned SHIPS_COUNT = 2;

	GameObjectSet _allGameObjects;
	GameObjectSet _obstacles;
	GameObjectSet _items;
	GameObjectSet _exitPoints;

	GameState _gameState;

	Ship * _ships[GameScreen::SHIPS_COUNT];
	ShipInfo _shipInfos[GameScreen::SHIPS_COUNT];

	GameObjectSet _prevFreeFallingItems;
	GameObjectSet _currFreeFallingItems;

	unsigned _iteration;

	GameRecorder _gameRecorder;

	void insertShip(Ship * ship, unsigned index);
	void removeShip(unsigned index);

	bool isGameOver() const;
	unsigned getActiveShipsCount() const;

	void setState(GameScreen::GameState newState);

public:
	GameScreen();
	~GameScreen();

	void addGameObject(Item * item);
	void addGameObject(Wall * wall);
	void addGameObject(ExitPoint * exit);
	void addGameObject(BigShip * ship);
	void addGameObject(SmallShip * ship);

	void setInitialState();
	void readUserInput(const Keyboard & keyboard);
	void process();
	void update(); //The actual moving/translation/advancment of the game
	void draw(Canvas & canvas) const;

	const GameRecorder & getRecorder() const;

	unsigned getIterations() const;
};

#endif /* GAME_H_ */