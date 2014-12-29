#include "GameScreen.h"

#include <string.h>
#include <assert.h>

#include "ScreenManager.h"
#include "GameAlgorithm.h"
#include "Game.h"
#include "InGameMenuScreen.h"

void GameScreen::insertShip(Ship * ship, unsigned index)
{
	this->_allGameObjects.insert(ship);
	this->_obstacles.insert(ship);

	this->_ships[index] = ship;

	this->_shipInfos[index]._velocity = Point::ZERO;
	this->_shipInfos[index]._rotate = false;
	this->_shipInfos[index]._pushPile.clear();
}

void GameScreen::removeShip(unsigned index)
{
	Ship * & ship = this->_ships[index];

	this->_allGameObjects.remove(ship);
	this->_obstacles.remove(ship);

	delete ship;
	ship = NULL;
}

void GameScreen::setInitialState()
{
	++this->_iteration;

	for(unsigned i = 0; i < GameScreen::SHIPS_COUNT; i++) //For each ship do:
	{
		this->_shipInfos[i]._pushPile.clear();
		this->_shipInfos[i]._velocity = Point::ZERO;
		this->_shipInfos[i]._rotate = false;
	}
}

void GameScreen::readUserInput(const Keyboard & keyboard)
{
	ShipInfo & smallShipInfo = this->_shipInfos[GameScreen::SMALL_SHIP_INDEX];
	ShipInfo & bigShipInfo = this->_shipInfos[GameScreen::BIG_SHIP_INDEX];

	if(keyboard.isPressed(keyboard.ESC))
	{
		ScreenManager::add(new InGameMenuScreen());
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
		this->_gameRecorder.recored(this->_iteration, smallShipAffectiveKey);
	}

	if(bigShipAffectiveKey != Keyboard::ESC)
	{
		this->_gameRecorder.recored(this->_iteration, bigShipAffectiveKey);
	}
}

void GameScreen::process()
{
	this->_prevFreeFallingItems = this->_currFreeFallingItems;

	unsigned totalShipsMass = 0;

	for(unsigned i = 0; i < GameScreen::SHIPS_COUNT; i++) //For each ship do:
	{
		Ship * const ship = this->_ships[i];
		if(ship == NULL) //Ship not present
		{
			continue; //Skip the not-present ship
		}

		ShipInfo & shipInfo = this->_shipInfos[i];

		if(GameAlgorithm::isShipCrashed(*ship, this->_prevFreeFallingItems))
		{
			ship->explode();
			this->setState(GameScreen::GAME_STATE_LOST);

			return; //Nothing more is going to happen to the now-crashed spaceship or to anything else in the game
		}

		GameAlgorithm::getTouchingObstacles(*ship, shipInfo._velocity, this->_obstacles, shipInfo._pushPile);
		if(shipInfo._pushPile.isPushable() == false)
		{
			shipInfo._velocity = Point::ZERO;
			shipInfo._pushPile.clear();
		}

		GameAlgorithm::expandToPushablePile(shipInfo._pushPile, this->_allGameObjects, shipInfo._velocity);

		totalShipsMass += ship->getMass();
	}

	ShipInfo & smallShipInfo = this->_shipInfos[GameScreen::SMALL_SHIP_INDEX];
	ShipInfo & bigShipInfo = this->_shipInfos[GameScreen::BIG_SHIP_INDEX];

	if(this->getActiveShipsCount() > 0 &&
		smallShipInfo._pushPile.isEqual(bigShipInfo._pushPile) &&
		smallShipInfo._velocity.equals(bigShipInfo._velocity) &&
		smallShipInfo._velocity.notEquals(Point::ZERO))
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
			Ship * & ship = this->_ships[i];
			if(ship == NULL) //Ship not present
			{
				continue; //Skip the not-present ship
			}

			ShipInfo & shipInfo = this->_shipInfos[i];

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
		Ship * & ship = this->_ships[i];
		if(ship == NULL) //Ship not present
		{
			continue; //Skip the not-present ship
		}

		ShipInfo & shipInfo = this->_shipInfos[i];

		if(shipInfo._velocity.notEquals(Point::ZERO))
		{
			shipInfo._pushPile.insert(ship);
			GameObjectSet pile(shipInfo._pushPile);
			GameAlgorithm::expandToPile(pile, this->_allGameObjects);
			pile.remove(shipInfo._pushPile);

			GameAlgorithm::removeBlockedFrom(pile, this->_allGameObjects, shipInfo._velocity);
			GameAlgorithm::expandToPile(shipInfo._pushPile, pile);
		}
		else if(shipInfo._rotate)
		{
			for(GameObjectSet::iterator iter = this->_items.begin(); iter != this->_items.end(); ++iter)
			{
				shipInfo._rotate &= (ship->isBlockedBy(**iter, Point::UP) == false);
			}

			for(GameObjectSet::iterator iter = this->_obstacles.begin(); iter != _obstacles.end(); ++iter)
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
		Ship * & ship = this->_ships[i];
		if(ship == NULL) //Ship not present
		{
			continue; //Skip the not-present ship
		}

		ShipInfo & shipInfo = this->_shipInfos[i];

		if(shipInfo._rotate)
		{
			ship->rotate();
		}
		else if(shipInfo._velocity.notEquals(Point::ZERO))
		{
			GameAlgorithm::move(shipInfo._pushPile, shipInfo._velocity);
		}

		if(GameAlgorithm::collidesWith(*ship, this->_exitPoints))
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
		this->_currFreeFallingItems.clear();
		for(GameObjectSet::iterator itemIter = this->_items.begin(); itemIter != this->_items.end(); ++itemIter)
		{
			if(GameAlgorithm::isBlocked(**itemIter, this->_obstacles, Point::DOWN) == false)
			{
				this->_currFreeFallingItems.insert(*itemIter);
			}
		}

		GameAlgorithm::move(this->_currFreeFallingItems, Point::DOWN);
	}
}

void GameScreen::draw(Canvas & canvas) const
{
	for(GameObjectSet::const_iterator iter = this->_allGameObjects.cbegin(); iter != this->_allGameObjects.cend(); ++iter)
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
	return this->_gameRecorder;
}

unsigned GameScreen::getIterations() const
{
	return this->_iteration + 1;
}

bool GameScreen::isGameOver() const
{
	return (this->_gameState != GameScreen::GAME_STATE_ONGOING);
}

unsigned GameScreen::getActiveShipsCount() const
{
	unsigned activeShipsCount = 0;
	for(unsigned i = 0; i < GameScreen::SHIPS_COUNT; i++)
	{
		if(this->_ships[i] != NULL)
		{
			++activeShipsCount;
		}
	}

	return activeShipsCount;
}

GameScreen::GameScreen()
{
	this->_gameState = GameScreen::GAME_STATE_ONGOING;
	this->_iteration = 0;

	for(unsigned i = 0; i < GameScreen::SHIPS_COUNT; ++i)
	{
		this->_ships[i] = NULL;
	}
};

GameScreen::~GameScreen()
{
	for(GameObjectSet::iterator iter = this->_allGameObjects.begin();
		iter != this->_allGameObjects.end(); ++iter)
	{
		delete *iter;
	}
}

void GameScreen::addGameObject(Item * item)
{
	this->_items.insert(item);
	this->_obstacles.insert(item);
	this->_allGameObjects.insert(item);
}

void GameScreen::addGameObject(Wall * wall)
{
	this->_obstacles.insert(wall);
	this->_allGameObjects.insert(wall);
}

void GameScreen::addGameObject(ExitPoint * exit)
{
	this->_exitPoints.insert(exit);
	this->_allGameObjects.insert(exit);
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

	this->_gameState = newState;
}
