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
	_shipInfos[index]._pullPile.clear();
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
		_shipInfos[i]._pullPile.clear();
		_shipInfos[i]._velocity = Point::ZERO;
		_shipInfos[i]._rotate = false;
	}

	_prevFreeFallingItems = _currFreeFallingItems;
	_currFreeFallingItems.clear();
	for(GameObjectSet::iterator itemIter = _items.begin(); itemIter != _items.end(); ++itemIter)
	{
		if(GameAlgorithm::isBlocked(**itemIter, _obstacles, Point::DOWN) == false)
		{
			_currFreeFallingItems += *itemIter;
		}
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

	if(keyboard.isPressed(Keyboard::I) || keyboard.isPressed(Keyboard::NUM8))
	{
		bigShipAffectiveKey = Keyboard::I;
		bigShipInfo._velocity = Point::UP;
	}
	else if(keyboard.isPressed(keyboard.M) || keyboard.isPressed(Keyboard::NUM2))
	{
		bigShipAffectiveKey = Keyboard::M;
		bigShipInfo._velocity = Point::DOWN;
	}
	else if(keyboard.isPressed(keyboard.J) || keyboard.isPressed(Keyboard::NUM4))
	{
		bigShipAffectiveKey = Keyboard::J;
		bigShipInfo._velocity = Point::LEFT;
	}
	else if(keyboard.isPressed(keyboard.L) || keyboard.isPressed(Keyboard::NUM6))
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

	//Bad ship crash detection
	{
		std::vector<BadShip *> crashedShips;

		for(GameObjectSet::iterator badShipIter = _badShips.begin();
			badShipIter != _badShips.end(); ++badShipIter)
		{
			BadShip * const badShip = (BadShip *)(*badShipIter);

			if(GameAlgorithm::isCrashed(*badShip, _prevFreeFallingItems))
			{
				crashedShips.push_back(badShip);
			}
		}

		for(unsigned i = 0; i < crashedShips.size(); ++i)
		{
			_allGameObjects -= crashedShips[i];
			_badShips -= crashedShips[i];
			_obstacles -= crashedShips[i];

			delete crashedShips[i];

		}
	}

	//Player ship crash detection
	for(unsigned i = 0; i < GameScreen::SHIPS_COUNT; i++)
	{
		Ship * const ship = _ships[i];
		if(ship == NULL) //Ship not present
		{
			continue; //Skip the not-present ship
		}

		if(GameAlgorithm::isCrashed(*ship, _prevFreeFallingItems))
		{
			ship->explode();
			shipExploded = true;
		}
	}

	if(shipExploded)
	{
		this->setState(GameScreen::GAME_STATE_LOST);

		return; //Nothing more is going to happen to the now-crashed spaceship or to anything else in the game
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
			//Get pull-pile
			GameObjectSet pullableItems(_items);
			GameObjectSet blockingObstacles(_obstacles);
			blockingObstacles -= ship;

			GameAlgorithm::removeBlockedFrom(pullableItems, blockingObstacles, shipInfo._velocity);
			const bool recursiveTouch = _isItemFrictionOn || (shipInfo._velocity == Point::DOWN) || (shipInfo._velocity == Point::UP);
			GameAlgorithm::getTouchingObstacles(*ship, Point::UP, pullableItems, shipInfo._pullPile, recursiveTouch);
			shipInfo._pullPile += ship;

			//Get push-pile
			GameAlgorithm::getTouchingObstacles(*ship, shipInfo._velocity, _obstacles, shipInfo._pushPile, true);

			if(_isItemFrictionOn)
			{
				GameAlgorithm::expandToPile(shipInfo._pushPile, _items);
			}

			if(shipInfo._pushPile.isPushable() == false)
			{
				shipInfo._velocity = Point::ZERO;
				shipInfo._pushPile.clear();
				shipInfo._pullPile.clear();
			}
			else
			{
				shipInfo._pushPile -= shipInfo._pullPile;
			}
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

	ShipInfo & smallShipInfo = _shipInfos[GameScreen::SMALL_SHIP_INDEX];
	ShipInfo & bigShipInfo = _shipInfos[GameScreen::BIG_SHIP_INDEX];

	if(this->getActiveShipsCount() > 0 &&
		(smallShipInfo._pushPile == bigShipInfo._pushPile) &&
		(smallShipInfo._velocity == bigShipInfo._velocity) &&
		(smallShipInfo._velocity != Point::ZERO))
	{
		unsigned totalShipsMass = 0;

		for(unsigned i = 0; i < GameScreen::SHIPS_COUNT; i++) //For each ship do:
		{
			Ship * const ship = _ships[i];
			if(ship == NULL) //Ship not present
			{
				continue; //Skip the not-present ship
			}

			totalShipsMass += ship->getMass();
		}

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
			GameAlgorithm::move(shipInfo._pullPile, shipInfo._velocity);
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
		for(GameObjectSet::iterator badShipIter = _badShips.begin();
			badShipIter != _badShips.end(); ++badShipIter)
		{
			GameObject & badShip = **badShipIter;
			const Ship & smallShip = *(_ships[GameScreen::SMALL_SHIP_INDEX]);
			const Ship & bigShip = *(_ships[GameScreen::BIG_SHIP_INDEX]);

			GameAlgorithm::updateBadSpaceshipPosition(badShip, smallShip, bigShip, _obstacles);
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

	canvas.printNotification((_isItemFrictionOn) ? "Slow push mode: On" : "Slow push mode: Off");

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
