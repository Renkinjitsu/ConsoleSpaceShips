#include "Game.h"

#include "io_utils.h"

#include "Direction.h"

#define ESC 27

bool Game::isBlockedByAny(const GameObject & gameObject, Direction from, const std::vector<GameObject *> & blockingObjects)
{
	for(std::vector<GameObject *>::const_iterator blockingObjectIter = blockingObjects.begin(); blockingObjectIter != blockingObjects.end(); ++blockingObjectIter)
	{
		if((*blockingObjectIter) != &gameObject)
		{
			if(gameObject.isBlockedBy(**blockingObjectIter, from))
			{
				if((*blockingObjectIter)->isPushable())
				{
					if(Game::isBlockedByAny(**blockingObjectIter, from, blockingObjects))
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

void Game::getPiledItems(const GameObject & gameObject, std::vector<Item *> & result) const
{
	for(std::vector<Item *>::const_iterator itemIter = this->_gameObjects._items.begin(); itemIter != this->_gameObjects._items.end(); ++itemIter)
	{
		if(&gameObject != *itemIter)
		{
			if(gameObject.isBlockedBy(**itemIter, DIRECTION_UP))
			{
				bool isAlreadyContained = false;
				for(size_t i = 0; i < result.size(); ++i)
				{
					if(result[i] == *itemIter)
					{
						isAlreadyContained = true;
						break;
					}
				}

				if(isAlreadyContained == false)
				{
					result.push_back(*itemIter);
					this->getPiledItems(**itemIter, result);
				}
			}
		}
	}

	//TODO: Remove duplicates
}

void Game::removeShip(Ship & ship)
{
	for(std::vector<GameObject *>::iterator iter = this->_gameObjects._all.begin(); iter != this->_gameObjects._all.end(); ++iter)
	{
		if(*iter == &ship)
		{
			this->_gameObjects._all.erase(iter);
			break;
		}
	}

	for(std::vector<GameObject *>::iterator iter = this->_gameObjects._blocking.begin(); iter != this->_gameObjects._blocking.end(); ++iter)
	{
		if(*iter == &ship)
		{
			this->_gameObjects._blocking.erase(iter);
			break;
		}
	}
}

void Game::moveItems(std::vector<Item *> & items, Direction direction)
{
	for(std::vector<Item *>::iterator item = items.begin(); item != items.end(); ++item)
	{
		bool isBlocked = false;
		for (std::vector<GameObject *>::const_iterator gameObject = this->_gameObjects._all.begin(); gameObject != this->_gameObjects._all.end(); ++gameObject)
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

void Game::setInitialState()
{
	for(unsigned i = 0; i < Game::SHIPS_COUNT; i++) //For each ship do:
	{
		ShipState & shipState = *(this->_updateArgs._shipStates[i]);

		if(shipState._ship.isPresent())
		{
			shipState._piledObjects.clear();
			shipState._shipDirection = DIRECTION_NONE;
		}
	}

	this->_updateArgs._rotateSmallShip = false;
}

void Game::readUserInput()
{
	Direction & smallShipDirection = this->_updateArgs._shipStates[Game::SMALL_SHIP_INDEX]->_shipDirection;
	Direction & bigShipDirection = this->_updateArgs._shipStates[Game::BIG_SHIP_INDEX]->_shipDirection;

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
	 * 1. Before advancment of items by gravity - get a list of all of the items that are not blocking the ship from above.
	 * 2. After the advancment: get a list of all of the items that do block the ship from above.
	 * 3. Get a list that is the intersection of the lists from step (1) & step (2).
	*/

	this->processUserInput();
	this->calculateChanges();
	this->applyChanges();
}

void Game::draw()
{
	this->_canvas.begin();

	for(std::vector<GameObject *>::iterator gameObjectIter = this->_gameObjects._all.begin(); gameObjectIter != this->_gameObjects._all.end(); ++gameObjectIter)
	{
		(*gameObjectIter)->draw(this->_canvas);
	}

	this->_canvas.end();
};

bool Game::isGameOver()
{
	bool isGameOver = this->_state._exit;

	for(unsigned i = 0; i < Game::SHIPS_COUNT && isGameOver == false; i++)
	{
		isGameOver |= (this->_gameObjects._smallShip.isAlive() == false);
	}

	isGameOver |= ((this->_gameObjects._smallShip.isPresent() == false) && (this->_gameObjects._bigShip.isPresent() == false)); //All ships are safe

	return isGameOver;
}

void Game::processUserInput()
{
	for(unsigned i = 0; i < Game::SHIPS_COUNT; i++) //For each ship do:
	{
		ShipState & shipState = *(this->_updateArgs._shipStates[i]);

		if(shipState._ship.isPresent())
		{
			if(this->isBlockedByAny(shipState._ship, shipState._shipDirection, this->_gameObjects._blocking))
			{
				shipState._shipDirection = DIRECTION_NONE;
			}

			if(shipState._shipDirection != DIRECTION_UP &&
				shipState._shipDirection != DIRECTION_NONE)
			{
				this->getPiledItems(shipState._ship, shipState._piledObjects);
			}
		}
	}
}

void Game::calculateChanges()
{
	if(this->_gameObjects._smallShip.isPresent())
	{
		this->_updateArgs._rotateSmallShip &= this->_gameObjects._smallShip.isRotationPossible(this->_gameObjects._blocking);
	}
}

void Game::applyChanges()
{
	if(this->_gameObjects._smallShip.isPresent())
	{
		if(this->_updateArgs._rotateSmallShip)
		{
			this->_gameObjects._smallShip.rotate();
		}
	}

	for(unsigned i = 0; i < Game::SHIPS_COUNT; i++)
	{
		ShipState & shipState = *(this->_updateArgs._shipStates[i]);

		if(shipState._ship.isPresent())
		{
			shipState._ship.move(shipState._shipDirection);
			this->moveItems(shipState._piledObjects, shipState._shipDirection); //Move items carried on ship

			if(this->_gameObjects._exitPoint->collidesWith(shipState._ship))
			{
				shipState._ship.disappear();
				this->removeShip(shipState._ship);
			}
		}
	}

	this->moveItems(this->_gameObjects._items, DIRECTION_DOWN); //Advance free-falling items
}

Game::Game(SmallShip & smallShip, BigShip & bigShip) : _canvas(), _gameObjects(smallShip, bigShip)
{
	this->_updateArgs._shipStates[Game::SMALL_SHIP_INDEX] = new ShipState(smallShip);
	this->_updateArgs._shipStates[Game::BIG_SHIP_INDEX] = new ShipState(bigShip);

	this->_gameObjects._all.push_back(&bigShip);
	this->_gameObjects._blocking.push_back(&bigShip);
	this->_gameObjects._all.push_back(&smallShip);
	this->_gameObjects._blocking.push_back(&smallShip);

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

		if(this->isGameOver() == false)
		{
			this->update();
		}

		if(this->isGameOver() == false)
		{
			this->draw();
		}

		Sleep(50);
	}

	this->_canvas.save();

	const char * endMessage = "Game OVER!";
	unsigned endMessageEffectIndex = 0;

	while(!(this->_state._exit))
	{
		this->_canvas.restore();
		this->_canvas.draw((Canvas::getWidth() - strlen(endMessage)) / 2, Canvas::getHeight() / 2, endMessage);
		this->_canvas.draw((Canvas::getWidth() - strlen(endMessage)) / 2 + endMessageEffectIndex, Canvas::getHeight() / 2, ' ');
		this->_canvas.end();

		endMessageEffectIndex = (endMessageEffectIndex + 1) % strlen(endMessage);
		Sleep(50);

		this->readUserInput();
	}
}

void Game::addGameObject(Item * item)
{
	this->_gameObjects._items.push_back(item);
	this->_gameObjects._all.push_back(item);
	this->_gameObjects._blocking.push_back(item);
}

void Game::addGameObject(Wall * wall)
{
	this->_gameObjects._walls.push_back(wall);
	this->_gameObjects._all.push_back(wall);
	this->_gameObjects._blocking.push_back(wall);
}

void Game::addGameObject(ExitPoint * exit)
{
	this->_gameObjects._exitPoint = exit;
	this->_gameObjects._all.push_back(exit);
}
