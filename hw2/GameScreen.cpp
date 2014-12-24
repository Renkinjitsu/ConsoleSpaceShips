#include "GameScreen.h"

#include <string.h>
#include <assert.h>

#include "Direction.h"

#include "ScreenManager.h"
#include "GameAlgorithm.h"
#include "Game.h"

void GameScreen::insertShip(Ship * ship)
{
	this->_allGameObjects.insert(ship);
	this->_obstacles.insert(ship);
}

void GameScreen::removeShip(Ship * ship)
{
	this->_allGameObjects.remove(ship);
	this->_obstacles.remove(ship);

	this->_objectsToDelete.insert(ship);
}

void GameScreen::setInitialState()
{
	for(unsigned i = 0; i < GameScreen::SHIPS_COUNT; i++) //For each ship do:
	{
		ShipState & shipState = this->_updateArgs._shipStates[i];

		shipState._pushPile.clear();
		shipState._direction = DIRECTION_NONE;
	}

	this->_updateArgs._rotateSmallShip = false;
}

void GameScreen::readUserInput(const Keyboard & keyboard)
{
	Direction & smallShipDirection = this->_updateArgs._shipStates[GameScreen::SMALL_SHIP_INDEX]._direction;
	Direction & bigShipDirection = this->_updateArgs._shipStates[GameScreen::BIG_SHIP_INDEX]._direction;

	if(keyboard.isPressed(keyboard.ESC))
	{
		this->exit(EXIT_TYPE_QUIT);
	}

	if(keyboard.isPressed(keyboard.Z))
	{
		this->_updateArgs._rotateSmallShip = true;
	}
	else if(keyboard.isPressed(keyboard.X))
	{
		smallShipDirection = DIRECTION_DOWN;
	}
	else if(keyboard.isPressed(keyboard.W))
	{
		smallShipDirection = DIRECTION_UP;
	}
	else if(keyboard.isPressed(keyboard.A))
	{
		smallShipDirection = DIRECTION_LEFT;
	}
	else if(keyboard.isPressed(keyboard.D))
	{
		smallShipDirection = DIRECTION_RIGHT;
	}

	if(keyboard.isPressed(keyboard.I))
	{
		bigShipDirection = DIRECTION_UP;
	}
	else if(keyboard.isPressed(keyboard.M))
	{
		bigShipDirection = DIRECTION_DOWN;
	}
	else if(keyboard.isPressed(keyboard.J))
	{
		bigShipDirection = DIRECTION_LEFT;
	}
	else if(keyboard.isPressed(keyboard.L))
	{
		bigShipDirection = DIRECTION_RIGHT;
	}
}

void GameScreen::process()
{
	this->_updateArgs._prevFreeFallingItems = this->_updateArgs._currFreeFallingItems;

	unsigned totalShipsMass = 0;

	for(unsigned i = 0; i < GameScreen::SHIPS_COUNT; i++) //For each ship do:
	{
		ShipState & shipState = this->_updateArgs._shipStates[i];
		Ship & ship = *shipState._ship;

		if(ship.isPresent())
		{
			if(GameAlgorithm::isShipCrashed(ship, this->_updateArgs._prevFreeFallingItems))
			{
				ship.explode();
				this->removeShip(&ship);
				this->exit(EXIT_TYPE_LOSE);
			}
			else
			{
				GameAlgorithm::getTouchingObstacles(ship, shipState._direction, this->_obstacles, shipState._pushPile);
				if(shipState._pushPile.isPushable() == false)
				{
					shipState._direction = DIRECTION_NONE;
					shipState._pushPile.clear();
				}

				if(shipState._direction != DIRECTION_NONE)
				{
					GameAlgorithm::expandToPushablePile(shipState._pushPile, this->_allGameObjects, shipState._direction);
				}

				totalShipsMass += ship.getMass();
			}
		}
	}

	ShipState & smallShipState = this->_updateArgs._shipStates[GameScreen::SMALL_SHIP_INDEX];
	ShipState & bigShipState = this->_updateArgs._shipStates[GameScreen::BIG_SHIP_INDEX];

	if(smallShipState._pushPile.isEqual(bigShipState._pushPile) &&
		smallShipState._direction == bigShipState._direction &&
		GameAlgorithm::isPushDirection(smallShipState._direction))
	{
		if(bigShipState._pushPile.getTotalMass() > totalShipsMass)
		{
			bigShipState._direction = smallShipState._direction = DIRECTION_NONE;
		}
		else
		{
			bigShipState._pushPile.clear(); //Both of the ships have the same pile
		}
	}
	else
	{
		for(unsigned i = 0; i < GameScreen::SHIPS_COUNT; i++)
		{
			ShipState & shipState = this->_updateArgs._shipStates[i];

			assert(shipState._pushPile.contains(shipState._ship) == false);

			if(GameAlgorithm::isPushDirection(shipState._direction) && shipState._ship->getMass() < shipState._pushPile.getTotalMass())
			{
				shipState._direction = DIRECTION_NONE;
			}
		}
	}

	for(unsigned i = 0; i < GameScreen::SHIPS_COUNT; i++) //For each ship do:
	{
		ShipState & shipState = this->_updateArgs._shipStates[i];
		Ship & ship = *shipState._ship;

		if(ship.isPresent() && shipState._direction != DIRECTION_NONE)
		{
			shipState._pushPile.insert(shipState._ship);
			GameObjectSet pile(shipState._pushPile);
			GameAlgorithm::expandToPile(pile, this->_allGameObjects);
			pile.remove(shipState._pushPile);

			for(GameObjectSet::iterator iter = pile.begin(); iter != pile.end(); ++iter)
			{
				{
					this->_updateArgs._rotateSmallShip &= (this->_smallShip->isBlockingRotation(**iter) == false);
				}
			}

			GameAlgorithm::removeBlockedFrom(pile, this->_allGameObjects, shipState._direction);
			GameAlgorithm::expandToPile(shipState._pushPile, pile);

		}
	}

	//Verify no item blocks the small ship from rotating
	if(this->_smallShip->isPresent() && this->_updateArgs._rotateSmallShip)
	{
		GameObjectSet carriedItems;
		GameAlgorithm::getPiledItems(*smallShipState._ship, carriedItems, this->_items);
		this->_updateArgs._rotateSmallShip &= carriedItems.isEmpty();

		for(GameObjectSet::const_iterator iter = this->_obstacles.begin();
			iter != this->_obstacles.end() &&
			this->_updateArgs._rotateSmallShip; ++iter)
		{
			if(*iter != this->_smallShip)
			{
				this->_updateArgs._rotateSmallShip &= (this->_smallShip->isBlockingRotation(**iter) == false);
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

	if(this->_smallShip->isPresent() &&
		this->_updateArgs._rotateSmallShip)
	{
		this->_smallShip->rotate();
	}

	for(unsigned i = 0; i < GameScreen::SHIPS_COUNT; i++)
	{
		ShipState & shipState = this->_updateArgs._shipStates[i];

		if(shipState._ship->isPresent())
		{
			if(shipState._direction != DIRECTION_NONE)
			{
				GameAlgorithm::move(shipState._pushPile, shipState._direction);
			}

			for(GameObjectSet::iterator iter = this->_exitPoints.begin();
				iter != this->_exitPoints.end(); ++iter)
			{
				if((*iter)->collidesWith(*shipState._ship))
				{
					shipState._ship->disappear();
					this->removeShip(shipState._ship);

					if(this->isGameOver())
					{
						ScreenManager::remove(this);
						Game::startNextLevel();
					}

					break;
				}
			}
		}
	}

	//Should be after the moveing of the push-piles
	this->_updateArgs._currFreeFallingItems.clear();
	for(GameObjectSet::iterator itemIter = this->_items.begin(); itemIter != this->_items.end(); ++itemIter) //List free-falling Item-s
	{
		if(GameAlgorithm::isBlocked(**itemIter, this->_obstacles, DIRECTION_DOWN) == false)
		{
			this->_updateArgs._currFreeFallingItems.insert(*itemIter);
		}
	}

	GameAlgorithm::move(this->_updateArgs._currFreeFallingItems, DIRECTION_DOWN);
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

bool GameScreen::isGameOver() const
{
	bool isGameOver = this->_state._exit;

	for(unsigned i = 0; i < GameScreen::SHIPS_COUNT && isGameOver == false; i++)
	{
		isGameOver |= (this->_updateArgs._shipStates[i]._ship->isAlive() == false);
	}

	isGameOver |= (this->_smallShip->isPresent() == false) && (this->_bigShip->isPresent() == false);

	return isGameOver;
}

GameScreen::GameScreen()
{
	this->_bigShip = NULL;
	this->_smallShip = NULL;

	this->_state._exit = false;
};

GameScreen::~GameScreen()
{
	for(GameObjectSet::iterator iter = this->_allGameObjects.begin();
		iter != this->_allGameObjects.end(); ++iter)
	{
		delete *iter;
	}

	for(GameObjectSet::iterator iter = this->_objectsToDelete.begin();
		iter != this->_objectsToDelete.end(); ++iter)
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

void GameScreen::exit(ExitType exitType)
{
	switch(exitType)
	{
		case EXIT_TYPE_LOSE:
		{
			ScreenManager::remove(this);
			Game::gameOver();
		}
		break;
	}

	ScreenManager::remove(this);
	this->_state._exit = true;
}

void GameScreen::addGameObject(BigShip * ship)
{
	assert(this->_bigShip == NULL);

	this->_bigShip = ship;
	this->_updateArgs._shipStates[GameScreen::BIG_SHIP_INDEX]._ship = ship;
	this->insertShip(ship);
}

void GameScreen::addGameObject(SmallShip * ship)
{
	assert(this->_smallShip == NULL);

	this->_smallShip = ship;
	this->_updateArgs._shipStates[GameScreen::SMALL_SHIP_INDEX]._ship = ship;
	this->insertShip(ship);
}
