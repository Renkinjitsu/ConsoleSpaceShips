#include "Game.h"

#include <string.h>

#include "io_utils.h"

#include "Direction.h"

#define ESC 27

bool Game::isBlockedByAny(const GameObject & gameObject, Direction from, const GameObjectSet & blockingObjects)
{
	GameObjectSet empty_ignore;

	return Game::isBlockedByAny(gameObject, from, blockingObjects, empty_ignore);
}

bool Game::isBlockedByAny(const GameObject & gameObject, Direction from, const GameObjectSet & blockingObjects, const GameObjectSet & ignore)
{
	for(GameObjectSet::const_iterator blockingObjectIter = blockingObjects.cbegin(); blockingObjectIter != blockingObjects.cend(); ++blockingObjectIter)
	{
		if((*blockingObjectIter) != &gameObject)
		{
			if(gameObject.isBlockedBy(**blockingObjectIter, from) && (ignore.contains(*blockingObjectIter) == false))
			{
				if((*blockingObjectIter)->isPushable())
				{
					if(Game::isBlockedByAny(**blockingObjectIter, from, blockingObjects, ignore))
					{
						return true;
					}
				}
				else
				{
					return true;
				}
			}
		}
	}

	return false;
}

void Game::getPiledItems(const GameObject & gameObject, GameObjectSet & result) const
{
	if(result.contains(&gameObject))
	{
		return;
	}

	for(GameObjectSet::const_iterator itemIter = this->_items.cbegin(); itemIter != this->_items.cend(); ++itemIter)
	{
		if(&gameObject != *itemIter && gameObject.isBlockedBy(**itemIter, DIRECTION_UP))
		{
			result.insertUnique(*itemIter);
			this->getPiledItems(**itemIter, result);
		}
	}
}

void Game::getPushPile(GameObject & currentPileElement, Direction direction, GameObjectSet & pileMembers) const
{
	if(this->isBlockedByAny(currentPileElement, direction, this->_blockingGameObjects, pileMembers))
	{
		return;
	}
	else if(pileMembers.contains(&currentPileElement))
	{
		return;
	}

	pileMembers.insert(&currentPileElement);

	for(GameObjectSet::const_iterator itemIter = this->_items.cbegin(); itemIter != this->_items.cend(); ++itemIter)
	{
		if(&currentPileElement != *itemIter)
		{
			bool shouldBePushed = currentPileElement.isBlockedBy(**itemIter, DIRECTION_UP);

			if(direction == DIRECTION_RIGHT || direction == DIRECTION_LEFT)
			{
				shouldBePushed |= currentPileElement.isBlockedBy(**itemIter, direction);
			}

			if(shouldBePushed)
			{
				Game::getPushPile(**itemIter, direction, pileMembers);
			}
		}
	}
}

void Game::removeShip(Ship & ship)
{
	this->_allGameObjects.remove(&ship);
	this->_blockingGameObjects.remove(&ship);
}

void Game::moveItems(GameObjectSet & items, Direction direction)
{
	for(GameObjectSet::iterator item = items.begin(); item != items.end(); ++item)
	{
		bool isBlocked = false;

		for(GameObjectSet::const_iterator gameObject = this->_allGameObjects.cbegin(); gameObject != this->_allGameObjects.end(); ++gameObject)
		{
			if((*item) != *gameObject && (*item)->isBlockedBy(*(*gameObject), direction))
			{
				isBlocked = true;
			}
		}

		if(isBlocked == false)
		{
			(*item)->move(direction);
		}
	}
}

void Game::listCrashPotentialItems(ShipState & shipState)
{
	shipState._crashPotentialItems.clear();

	for(GameObjectSet::iterator itemIter = this->_items.begin(); itemIter != this->_items.end(); ++itemIter)
	{
		if((*itemIter)->isBlockedBy(shipState._ship, DIRECTION_DOWN) == false)
		{
			shipState._crashPotentialItems.insert(*itemIter);
		}
	}
}

void Game::listFreeFallingItems()
{
	this->_updateArgs._freeFallingItems.clear();

	for(GameObjectSet::iterator itemIter = this->_items.begin(); itemIter != this->_items.end(); ++itemIter)
	{
		if(Game::isBlockedByAny(**itemIter, DIRECTION_DOWN, this->_blockingGameObjects) == false)
		{
			this->_updateArgs._freeFallingItems.insert(*itemIter);
		}
	}
}

void Game::refineCrashPotentialItems(ShipState & shipState)
{
	GameObjectSet::iterator potentialItem = shipState._crashPotentialItems.begin();

	while(potentialItem != shipState._crashPotentialItems.end())
	{
		if(shipState._ship.isBlockedBy(**potentialItem, DIRECTION_UP) == false ||
			this->_updateArgs._freeFallingItems.contains(*potentialItem) == false)
		{
			shipState._crashPotentialItems.remove(*potentialItem);
			potentialItem = shipState._crashPotentialItems.begin();
		}
		else
		{
			++potentialItem;
		}
	}
}

void Game::expandCrashPotentialItems(ShipState & shipState)
{
	for(GameObjectSet::iterator iter = shipState._crashPotentialItems.begin(); iter != shipState._crashPotentialItems.end(); ++iter)
	{
		this->getPiledItems(**iter, shipState._crashPotentialItems);
	}
}

unsigned Game::getTotalMass(const GameObjectSet & gameObjects) const
{
	unsigned result = 0;

	for(GameObjectSet::const_iterator iter = gameObjects.cbegin(); iter != gameObjects.cend(); ++iter)
	{
		result += (*iter)->getMass();
	}

	return result;
}

void Game::setInitialState()
{
	for(unsigned i = 0; i < Game::SHIPS_COUNT; i++) //For each ship do:
	{
		ShipState & shipState = *(this->_updateArgs._shipStates[i]);

		shipState._pushPile.clear();

		if(shipState._ship.isPresent())
		{
			shipState._direction = DIRECTION_NONE;
		}

		this->listCrashPotentialItems(shipState);
	}

	this->_updateArgs._rotateSmallShip = false;
}

void Game::readUserInput()
{
	Direction & smallShipDirection = this->_updateArgs._shipStates[Game::SMALL_SHIP_INDEX]->_direction;
	Direction & bigShipDirection = this->_updateArgs._shipStates[Game::BIG_SHIP_INDEX]->_direction;

	while(_kbhit())
	{
		switch (_getch())
		{
			case ESC:
			{
				this->_state._exit = true;
			}
			break;

			case 'x':
			{
				smallShipDirection = DIRECTION_DOWN;
			}
			break;

			case 'w':
			{
				smallShipDirection = DIRECTION_UP;
			}
			break;

			case 'a':
			{
				smallShipDirection = DIRECTION_LEFT;
			}
			break;

			case 'd':
			{
				smallShipDirection = DIRECTION_RIGHT;
			}
			break;

			case 'z':
			{
				this->_updateArgs._rotateSmallShip = true;
			}
			break;

			case 'i':
			{
				bigShipDirection = DIRECTION_UP;
			}
			break;

			case 'm':
			{
				bigShipDirection = DIRECTION_DOWN;
			}
			break;

			case 'j':
			{
				bigShipDirection = DIRECTION_LEFT;
			}
			break;

			case 'l':
			{
				bigShipDirection = DIRECTION_RIGHT;
			}
			break;
		} //End of 'switch'
	} //End of loop
}

void Game::update()
{
	/*
	 * Items that falls on (A.K.A "crash-into") each ship are detected thus:
	 *
	 * 1. List crash-potential-items (items that do not block the ship from above).
	 * 2. List 'free-falling' items.
	 * 3. From the crash-potential-items, remove the items that are not 'free-falling'.
	 * 4. From the crash-potential-items, remove the items that do not block the ship from above.
	 * 5. Expand crash-potential-items to 'crash-pile' by piling all contained items.
	 * 6. Calculate the mass of the crash-pile.
	 * 7. If the mass is too great, explode the ship.
	*/

	this->processUserInput();
	this->applyChanges();
}

void Game::draw()
{
	this->_canvas.begin();

	for(GameObjectSet::const_iterator iter = this->_allGameObjects.cbegin(); iter != this->_allGameObjects.cend(); ++iter)
	{
		(*iter)->draw(this->_canvas);
	}

	this->_canvas.end();
}

void Game::gameOverScreen()
{
	this->_canvas.save();

	const char * const endMessage = "Game OVER!";
	unsigned endMessageEffectIndex = 0;

	const char * const explodedShipAnimation = "*+X";
	unsigned explodedShipAnimationIndex = 0;

	Ship * ships[Game::SHIPS_COUNT];
	ships[0] = &(this->_smallShip);
	ships[1] = &(this->_bigShip);

	while(!(this->_state._exit))
	{
		this->_canvas.restore();

		for(unsigned i = 0; i < Game::SHIPS_COUNT; i++)
		{
			Ship & ship = *(ships[i]);

			if(ship.isAlive() == false)
			{
				ship.setTexture(explodedShipAnimation[explodedShipAnimationIndex]);
				ship.draw(this->_canvas);
			}
		}

		this->_canvas.draw((Canvas::getWidth() - strlen(endMessage)) / 2, Canvas::getHeight() / 2, endMessage);
		this->_canvas.draw((Canvas::getWidth() - strlen(endMessage)) / 2 + endMessageEffectIndex, Canvas::getHeight() / 2, ' ');

		this->_canvas.end();

		endMessageEffectIndex = (endMessageEffectIndex + 1) % strlen(endMessage);
		explodedShipAnimationIndex = (explodedShipAnimationIndex + 1) % strlen(explodedShipAnimation);

		Sleep(75);

		this->readUserInput();
	}
}

bool Game::isGameOver() const
{
	bool isGameOver = this->_state._exit;

	for(unsigned i = 0; i < Game::SHIPS_COUNT && isGameOver == false; i++)
	{
		isGameOver |= (this->_updateArgs._shipStates[i]->_ship.isAlive() == false);
	}

	isGameOver |= (this->_smallShip.isPresent() == false) && (this->_bigShip.isPresent() == false);

	return isGameOver;
}

void Game::processUserInput()
{
	unsigned totalShipsMass = 0;

	for(unsigned i = 0; i < Game::SHIPS_COUNT; i++) //For each ship do:
	{
		ShipState & shipState = *(this->_updateArgs._shipStates[i]);
		GameObjectSet & items = this->_items;

		if(shipState._ship.isPresent())
		{
			if(this->isBlockedByAny(shipState._ship, shipState._direction, this->_blockingGameObjects))
			{
				shipState._direction = DIRECTION_NONE;
			}
			else
			{
				if(shipState._direction == DIRECTION_LEFT || shipState._direction == DIRECTION_RIGHT)
				{
					for(GameObjectSet::iterator itemIter = this->_items.begin(); itemIter != this->_items.end(); ++itemIter)
					{
						if(shipState._ship.isBlockedBy(**itemIter, shipState._direction))
						{
							this->getPushPile(**itemIter, shipState._direction, shipState._pushPile);
						}
					}
				}
			}

			totalShipsMass += shipState._ship.getMass();
		}
	}

	ShipState & smallShipState = *(this->_updateArgs._shipStates[Game::SMALL_SHIP_INDEX]);
	ShipState & bigShipState = *(this->_updateArgs._shipStates[Game::BIG_SHIP_INDEX]);

	if(smallShipState._pushPile.isEqual(bigShipState._pushPile))
	{
		if(totalShipsMass < Game::getTotalMass(bigShipState._pushPile))
		{
			bigShipState._direction = smallShipState._direction = DIRECTION_NONE;
		}
	}
	else
	{
		for(unsigned i = 0; i < Game::SHIPS_COUNT; i++)
		{
			ShipState & shipState = *(this->_updateArgs._shipStates[i]);

			if(shipState._ship.getMass() < Game::getTotalMass(shipState._pushPile))
			{
				shipState._direction = DIRECTION_NONE;
			}
		}
	}

	//Verify no item blocks the small ship from rotating
	if(this->_smallShip.isPresent() && this->_updateArgs._rotateSmallShip)
	{
		for(GameObjectSet::const_iterator iter = this->_blockingGameObjects.begin(); iter != this->_blockingGameObjects.end(); iter++)
		{
			if(*iter != &this->_smallShip)
			{
				this->_updateArgs._rotateSmallShip &= (this->_smallShip.isBlockingRotation(**iter) == false);
			}
		}
	}
}

void Game::applyChanges()
{
	if(this->_smallShip.isPresent())
	{
		if(this->_updateArgs._rotateSmallShip)
		{
			this->_smallShip.rotate();
		}
	}

	for(unsigned i = 0; i < Game::SHIPS_COUNT; i++)
	{
		ShipState & shipState = *(this->_updateArgs._shipStates[i]);

		if(shipState._ship.isPresent())
		{
			if(shipState._direction != DIRECTION_NONE)
			{
				GameObjectSet pileMembers;
				this->getPushPile(shipState._ship, shipState._direction, pileMembers);

				for(GameObjectSet::iterator iter = pileMembers.begin(); iter != pileMembers.end(); iter++)
				{
					(*iter)->move(shipState._direction);
				}
			}

			if(this->_exitPoint->collidesWith(shipState._ship))
			{
				shipState._ship.disappear();
				this->removeShip(shipState._ship);
			}
		}
	}

	this->listFreeFallingItems();
	this->moveItems(this->_items, DIRECTION_DOWN); //Advance free-falling items

	for(unsigned i = 0; i < Game::SHIPS_COUNT; i++)
	{
		ShipState & shipState = *(this->_updateArgs._shipStates[i]);

		if(shipState._ship.isPresent())
		{
			this->refineCrashPotentialItems(shipState);
			this->expandCrashPotentialItems(shipState);

			if(this->getTotalMass(shipState._crashPotentialItems) >= (shipState._ship.getMass() / 2))
			{
				shipState._ship.explode();
			}
		}
	}
}

Game::Game(Canvas & _canvas, SmallShip & smallShip, BigShip & bigShip) : _canvas(_canvas), _smallShip(smallShip), _bigShip(bigShip)
{
	this->_updateArgs._shipStates[Game::SMALL_SHIP_INDEX] = new ShipState(smallShip);
	this->_updateArgs._shipStates[Game::BIG_SHIP_INDEX] = new ShipState(bigShip);

	this->_allGameObjects.insert(&bigShip);
	this->_blockingGameObjects.insert(&bigShip);
	this->_allGameObjects.insert(&smallShip);
	this->_blockingGameObjects.insert(&smallShip);

	this->_state._exit = false;
};

Game::~Game()
{
	for(unsigned i = 0; i < Game::SHIPS_COUNT; i++)
	{
		delete this->_updateArgs._shipStates[i];
	}
}

void Game::run()
{
	while(this->isGameOver() == false)
	{
		this->setInitialState();
		this->readUserInput();
		this->update();
		this->draw();

		Sleep(50);
	}

	this->gameOverScreen();
}

void Game::addGameObject(Item * item)
{
	this->_items.insert(item);
	this->_allGameObjects.insert(item);
	this->_blockingGameObjects.insert(item);
}

void Game::addGameObject(Wall * wall)
{
	this->_allGameObjects.insert(wall);
	this->_blockingGameObjects.insert(wall);
}

void Game::addGameObject(ExitPoint * exit)
{
	this->_exitPoint = exit;
	this->_allGameObjects.insert(exit);
}
