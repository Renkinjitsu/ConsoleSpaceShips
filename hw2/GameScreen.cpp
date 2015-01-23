#include "GameScreen.h"

#include <string.h>
#include <assert.h>

#include "ScreenManager.h"
#include "GameAlgorithm.h"
#include "Game.h"
#include "InGameMenuScreen.h"

void GameScreen::insertShip(Ship * ship, unsigned index)
{
	_allGameObjects += ship;
	_obstacles += ship;

	_ships[index] = ship;

	_shipInfos[index]._velocity = Point::ZERO;
	_shipInfos[index]._rotate = false;
	_shipInfos[index]._pushPile.clear();
}

void GameScreen::removeShip(unsigned index)
{
	Ship * & ship = _ships[index];

	_allGameObjects -= ship;
	_obstacles -= ship;

	delete ship;
	ship = NULL;
}

void GameScreen::setInitialState()
{
	++_iteration;

	for(unsigned i = 0; i < GameScreen::SHIPS_COUNT; i++) //For each ship do:
	{
		_shipInfos[i]._pushPile.clear();
		_shipInfos[i]._velocity = Point::ZERO;
		_shipInfos[i]._rotate = false;
	}
}

void GameScreen::readUserInput(const Keyboard & keyboard)
{
	ShipInfo & smallShipInfo = _shipInfos[GameScreen::SMALL_SHIP_INDEX];
	ShipInfo & bigShipInfo = _shipInfos[GameScreen::BIG_SHIP_INDEX];

	if(keyboard.isPressed(keyboard.ESC))
	{
		ScreenManager::add(new InGameMenuScreen());
	}

	if(keyboard.isPressed(Keyboard::Key::B))
	{
		_isItemFrictionOn = !_isItemFrictionOn;

		_gameRecorder.recored(_iteration, Keyboard::Key::B);
	}

	Keyboard::Key smallShipAffectiveKey = Keyboard::ESC;
	Keyboard::Key bigShipAffectiveKey = Keyboard::ESC;

	if(keyboard.isPressed(Keyboard::Z))
	{
		smallShipAffectiveKey = Keyboard::Z;
		smallShipInfo._rotate = true;
	}
	else if(keyboard.isPressed(keyboard.X))
	{
		smallShipAffectiveKey = Keyboard::X;
		smallShipInfo._velocity = Point::DOWN;
	}
	else if(keyboard.isPressed(keyboard.W))
	{
		smallShipAffectiveKey = Keyboard::W;
		smallShipInfo._velocity = Point::UP;
	}
	else if(keyboard.isPressed(keyboard.A))
	{
		smallShipAffectiveKey = Keyboard::A;
		smallShipInfo._velocity = Point::LEFT;
	}
	else if(keyboard.isPressed(keyboard.D))
	{
		smallShipAffectiveKey = Keyboard::D;
		smallShipInfo._velocity = Point::RIGHT;
	}

	if(keyboard.isPressed(Keyboard::I))
	{
		bigShipAffectiveKey = Keyboard::I;
		bigShipInfo._velocity = Point::UP;
	}
	else if(keyboard.isPressed(keyboard.M))
	{
		bigShipAffectiveKey = Keyboard::M;
		bigShipInfo._velocity = Point::DOWN;
	}
	else if(keyboard.isPressed(keyboard.J))
	{
		bigShipAffectiveKey = Keyboard::J;
		bigShipInfo._velocity = Point::LEFT;
	}
	else if(keyboard.isPressed(keyboard.L))
	{
		bigShipAffectiveKey = Keyboard::L;
		bigShipInfo._velocity = Point::RIGHT;
	}

	if(keyboard.isPressed(Keyboard::P))
	{
		this->setState(GameScreen::GAME_STATE_WON);
	}

	if(smallShipAffectiveKey != Keyboard::ESC)
	{
		_gameRecorder.recored(_iteration, smallShipAffectiveKey);
	}

	if(bigShipAffectiveKey != Keyboard::ESC)
	{
		_gameRecorder.recored(_iteration, bigShipAffectiveKey);
	}
}

void GameScreen::process()
{
	_prevFreeFallingItems = _currFreeFallingItems;

	bool shipExploded = false;

	{
		GameObjectSet detonatedBombs;
		GameObjectSet affectedObjects;
		GameAlgorithm::handleBombs(detonatedBombs, affectedObjects, _bombs, _obstacles);

		_allGameObjects -= detonatedBombs;
		_bombs -= detonatedBombs;

		(_allGameObjects -= affectedObjects) += _exitPoints;
		_badShips -= affectedObjects;
		_currFreeFallingItems -= affectedObjects;
		_prevFreeFallingItems -= affectedObjects;
		_items -= affectedObjects;
		_obstacles -= affectedObjects;

		//Detect exploded ships
		for(unsigned i = 0; i < GameScreen::SHIPS_COUNT; i++)
		{
			if(affectedObjects.contains(_ships[i]))
			{
				affectedObjects -= _ships[i];
				_ships[i]->explode();
				shipExploded = true;
			}
		}

		for(GameObjectSet::iterator iter = detonatedBombs.begin();
			iter != detonatedBombs.end();
			++iter)
		{
			delete *iter;
		}

		for(GameObjectSet::iterator iter = affectedObjects.begin();
			iter != affectedObjects.end();
			++iter)
		{
			delete *iter;
		}
	}

	//Detect player & enemy troop contact
	for(unsigned i = 0; i < GameScreen::SHIPS_COUNT; i++)
	{
		for(std::vector<GameObject *>::iterator enemyIter = _badShips.begin();
			enemyIter != _badShips.end() && _ships[i];
			++enemyIter)
		{
			BadShip * enemyTroop = (BadShip *)(*enemyIter);

			if(_ships[i]->isTouching(*enemyTroop))
			{
				_ships[i]->explode();
				shipExploded = true;
			}
		}
	}

	if(shipExploded)
	{
		this->setState(GameScreen::GAME_STATE_LOST);

		return; //Nothing more is going to happen to the now-crashed spaceship or to anything else in the game
	}

	unsigned totalShipsMass = 0;

	for(unsigned i = 0; i < GameScreen::SHIPS_COUNT; i++) //For each ship do:
	{
		Ship * const ship = _ships[i];
		if(ship == NULL) //Ship not present
		{
			continue; //Skip the not-present ship
		}

		ShipInfo & shipInfo = _shipInfos[i];

		if(GameAlgorithm::isShipCrashed(*ship, _prevFreeFallingItems))
		{
			ship->explode();
			this->setState(GameScreen::GAME_STATE_LOST);

			return; //Nothing more is going to happen to the now-crashed spaceship or to anything else in the game
		}

		GameAlgorithm::getTouchingObstacles(*ship, shipInfo._velocity, _obstacles, shipInfo._pushPile);
		if(shipInfo._pushPile.isPushable() == false)
		{
			shipInfo._velocity = Point::ZERO;
			shipInfo._pushPile.clear();
		}

		GameAlgorithm::expandToPushablePile(shipInfo._pushPile, _allGameObjects, shipInfo._velocity);

		totalShipsMass += ship->getMass();
	}

	ShipInfo & smallShipInfo = _shipInfos[GameScreen::SMALL_SHIP_INDEX];
	ShipInfo & bigShipInfo = _shipInfos[GameScreen::BIG_SHIP_INDEX];

	if(this->getActiveShipsCount() > 0 &&
		(smallShipInfo._pushPile == bigShipInfo._pushPile) &&
		(smallShipInfo._velocity == bigShipInfo._velocity) &&
		(smallShipInfo._velocity != Point::ZERO))
	{
		if(bigShipInfo._pushPile.getTotalMass() > totalShipsMass)
		{
			bigShipInfo._velocity = smallShipInfo._velocity = Point::ZERO;
		}
		else
		{
			bigShipInfo._pushPile.clear(); //Both of the ships have the same pile,
		}
	}
	else
	{
		for(unsigned i = 0; i < GameScreen::SHIPS_COUNT; i++)
		{
			Ship * & ship = _ships[i];
			if(ship == NULL) //Ship not present
			{
				continue; //Skip the not-present ship
			}

			ShipInfo & shipInfo = _shipInfos[i];

			assert(shipInfo._pushPile.contains(ship) == false);

			/*
			 * The test of the following expresion is valid only if 'shipState._direction' not equals 'Point::Zero',
			 *  but it isn't tested, because there is no side effect to a 'false-positive'.
			 */
			if(ship->getMass() < shipInfo._pushPile.getTotalMass())
			{
				shipInfo._velocity = Point::ZERO;
			}
		}
	}

	for(unsigned i = 0; i < GameScreen::SHIPS_COUNT; i++)
	{
		Ship * & ship = _ships[i];
		if(ship == NULL) //Ship not present
		{
			continue; //Skip the not-present ship
		}

		ShipInfo & shipInfo = _shipInfos[i];

		if(shipInfo._velocity != Point::ZERO)
		{
			shipInfo._pushPile += ship;
			GameObjectSet pile(shipInfo._pushPile);
			GameAlgorithm::expandToPile(pile, _allGameObjects);
			pile -= shipInfo._pushPile;

			GameAlgorithm::removeBlockedFrom(pile, _allGameObjects, shipInfo._velocity);
			GameAlgorithm::expandToPile(shipInfo._pushPile, pile);
		}
		else if(shipInfo._rotate)
		{
			for(GameObjectSet::iterator iter = _items.begin(); iter != _items.end(); ++iter)
			{
				shipInfo._rotate &= (ship->isBlockedBy(**iter, Point::UP) == false);
			}

			for(GameObjectSet::iterator iter = _obstacles.begin(); iter != _obstacles.end(); ++iter)
			{
				if(ship != *iter)
				{
					shipInfo._rotate &= (ship->isBlockingRotation(**iter) == false);
				}
			}
		}
	}
}

void GameScreen::update()
{
	if(this->isGameOver())
	{
		return;
	}

	for(unsigned i = 0; i < GameScreen::SHIPS_COUNT; i++)
	{
		Ship * & ship = _ships[i];
		if(ship == NULL) //Ship not present
		{
			continue; //Skip the not-present ship
		}

		ShipInfo & shipInfo = _shipInfos[i];

		if(shipInfo._rotate)
		{
			ship->rotate();
		}
		else if(shipInfo._velocity != Point::ZERO)
		{
			GameAlgorithm::move(shipInfo._pushPile, shipInfo._velocity);
		}

		if(GameAlgorithm::collidesWith(*ship, _exitPoints))
		{
			this->removeShip(i);
		}
	}

	if(this->getActiveShipsCount() == 0)
	{
		this->setState(GameScreen::GAME_STATE_WON);
	}
	else //Nope, the show goes on!
	{
		//List free-falling Item-s (should be after the moving of the push-piles)
		_currFreeFallingItems.clear();
		for(GameObjectSet::iterator itemIter = _items.begin(); itemIter != _items.end(); ++itemIter)
		{
			if(GameAlgorithm::isBlocked(**itemIter, _obstacles, Point::DOWN) == false)
			{
				_currFreeFallingItems += *itemIter;
			}
		}

		//Update bad spaceships position
		for(GameObjectSet::iterator badShipIter = _badShips.begin();
			badShipIter != _badShips.end(); ++badShipIter)
		{
			GameObject & badShip = **badShipIter;
			const Point badShipPosition = badShip.getTopLeft();

			const Ship & smallShip = *(_ships[GameScreen::SMALL_SHIP_INDEX]);
			const Ship & bigShip = *(_ships[GameScreen::BIG_SHIP_INDEX]);

			const Point smallShipClosestPoint = smallShip.getClosestStepDistancePoint(badShip);
			const Point bigShipClosestPoint = bigShip.getClosestStepDistancePoint(badShip);

			const unsigned smallShipDistance = badShip.getStepDistance(smallShipClosestPoint);
			const unsigned bigShipDistance = badShip.getStepDistance(bigShipClosestPoint);

			const Point & targetPoint = (bigShipDistance <= smallShipDistance) ? bigShipClosestPoint : smallShipClosestPoint;

			Point verticalMovement = Point::ZERO;
			if(targetPoint.isAbove(badShipPosition))
			{
				if(GameAlgorithm::isBlocked(badShip, _obstacles, Point::UP) == false)
				{
					verticalMovement = Point::UP;
				}
			}
			else if(targetPoint.isBelow(badShipPosition))
			{
				if(GameAlgorithm::isBlocked(badShip, _obstacles, Point::DOWN) == false)
				{
					verticalMovement = Point::DOWN;
				}
			}
			//else, no horizontal movement

			Point horisontalMovement = Point::ZERO;
			if(targetPoint.isLeftOf(badShipPosition))
			{
				if(GameAlgorithm::isBlocked(badShip, _obstacles, Point::LEFT) == false)
				{
					horisontalMovement = Point::LEFT;
				}
			}
			else if(targetPoint.isRightOf(badShipPosition))
			{
				if(GameAlgorithm::isBlocked(badShip, _obstacles, Point::RIGHT) == false)
				{
					horisontalMovement = Point::RIGHT;
				}
			}
			//else, no vertical movement

			if(horisontalMovement == Point::ZERO)
			{
				badShip.move(verticalMovement);
			}
			else if(verticalMovement == Point::ZERO)
			{
				badShip.move(horisontalMovement);
			}
			else if(badShipPosition.getHorizontalDistance(targetPoint) >= badShipPosition.getVerticalDistance(targetPoint))
			{
				badShip.move(horisontalMovement);
			}
			else
			{
				badShip.move(verticalMovement);
			}
		}

		GameAlgorithm::move(_currFreeFallingItems, Point::DOWN);
		GameAlgorithm::move(_bombs, Point::DOWN);
	}
}

void GameScreen::draw(Canvas & canvas) const
{
	for(GameObjectSet::const_iterator iter = _allGameObjects.cbegin(); iter != _allGameObjects.cend(); ++iter)
	{
		(*iter)->draw(canvas);
	}

	if(this->isGameOver())
	{
		canvas.save();
	}
}

const GameRecorder & GameScreen::getRecorder() const
{
	return _gameRecorder;
}

unsigned GameScreen::getIterations() const
{
	return _iteration + 1;
}

bool GameScreen::isGameOver() const
{
	return (_gameState != GameScreen::GAME_STATE_ONGOING);
}

unsigned GameScreen::getActiveShipsCount() const
{
	unsigned activeShipsCount = 0;
	for(unsigned i = 0; i < GameScreen::SHIPS_COUNT; i++)
	{
		if(_ships[i] != NULL)
		{
			++activeShipsCount;
		}
	}

	return activeShipsCount;
}

GameScreen::GameScreen()
{
	_gameState = GameScreen::GAME_STATE_ONGOING;
	_isItemFrictionOn = false;
	_iteration = 0;

	for(unsigned i = 0; i < GameScreen::SHIPS_COUNT; ++i)
	{
		_ships[i] = NULL;
	}
};

GameScreen::~GameScreen()
{
	for(GameObjectSet::iterator iter = _allGameObjects.begin();
		iter != _allGameObjects.end(); ++iter)
	{
		delete *iter;
	}
}

void GameScreen::addGameObject(Item * item)
{
	_items += item;
	_obstacles += item;
	_allGameObjects += item;
}

void GameScreen::addGameObject(Wall * wall)
{
	_obstacles += wall;
	_allGameObjects += wall;
}

void GameScreen::addGameObject(ExitPoint * exit)
{
	_exitPoints += exit;
	_allGameObjects += exit;
}

void GameScreen::addGameObject(Bomb * bomb)
{
	_bombs += bomb;
	_allGameObjects += bomb;
}

void GameScreen::addGameObject(BadShip * ship)
{
	_badShips += ship;
	_allGameObjects += ship;
	_obstacles += ship;
}

void GameScreen::addGameObject(BigShip * ship)
{
	this->insertShip(ship, GameScreen::BIG_SHIP_INDEX);
}

void GameScreen::addGameObject(SmallShip * ship)
{
	this->insertShip(ship, GameScreen::SMALL_SHIP_INDEX);
}

void GameScreen::setState(GameScreen::GameState newState)
{
	switch(newState)
	{
		case GameScreen::GAME_STATE_LOST:
		{
			ScreenManager::remove(this);
			Game::gameOver();
		}
		break;

		case GameScreen::GAME_STATE_WON:
		{
			ScreenManager::remove(this);
			Game::startNextLevel();
		}
		break;

		case GameScreen::GAME_STATE_QUIT:
		{
			ScreenManager::remove(this);
		}
		break;

		default:
		{
			//Nothing to do
		}
		break;
	}

	_gameState = newState;
}
