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
	for(std::vector<Item *>::const_iterator itemIter = this->_items.begin(); itemIter != this->_items.end(); ++itemIter)
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

void Game::moveItems(std::vector<Item *> & items, Direction direction)
{
	for(std::vector<Item *>::iterator item = items.begin(); item != items.end(); ++item)
	{
		bool isBlocked = false;
		for (std::vector<GameObject *>::const_iterator gameObject = _allGameObjects.begin(); gameObject != _allGameObjects.end(); ++gameObject)
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

void Game::draw_all()
{
	this->_canvas.begin();

	for(std::vector<GameObject *>::iterator gameObjectIter = this->_allGameObjects.begin(); gameObjectIter != this->_allGameObjects.end(); ++gameObjectIter)
	{
		(*gameObjectIter)->draw(this->_canvas);
	}

	this->_canvas.end();
};

void Game::readUserInput()
{
	this->_bigShipDirection = DIRECTION_NONE;
	this->_smallShipDirection = DIRECTION_NONE;
	this->_smallShipTryRotate = false;

	while(_kbhit())
	{
		switch (_getch())
		{
			case ESC:
			{
				this->_exit = true;
			}
			break;

			case 'x':
			{
				this->_smallShipDirection = DIRECTION_DOWN;
			}
			break;

			case 'w':
			{
				this->_smallShipDirection = DIRECTION_UP;
			}
			break;

			case 'a':
			{
				this->_smallShipDirection = DIRECTION_LEFT;
			}
			break;

			case 'd':
			{
				this->_smallShipDirection = DIRECTION_RIGHT;
			}
			break;

			case 'z':
			{
				this->_smallShipTryRotate = true;
			}
			break;

			case 'i':
			{
				this->_bigShipDirection = DIRECTION_UP;
			}
			break;

			case 'm':
			{
				this->_bigShipDirection = DIRECTION_DOWN;
			}
			break;

			case 'j':
			{
				this->_bigShipDirection = DIRECTION_LEFT;
			}
			break;

			case 'l':
			{
				this->_bigShipDirection = DIRECTION_RIGHT;
			}
			break;
		} //End of 'switch'
	} //End of loop
}

void Game::update()
{
	if(this->_smallShipTryRotate)
	{
		if(this->_smallShip->isRotationPossible(this->_allBlockingObjects))
		{
			this->_smallShip->rotate();
		}
	}

	std::vector<Item *> smallShipPiledObjects;
	std::vector<Item *> bigShipPiledObjects;

	this->getPiledItems(*(this->_smallShip), smallShipPiledObjects);
	this->getPiledItems(*(this->_bigShip), bigShipPiledObjects);

	if(this->isBlockedByAny(*(this->_smallShip), this->_smallShipDirection, this->_allBlockingObjects))
	{
		this->_smallShipDirection = DIRECTION_NONE;
	}

	if(this->isBlockedByAny(*(this->_bigShip), this->_bigShipDirection, this->_allBlockingObjects))
	{
		this->_bigShipDirection = DIRECTION_NONE;
	}

	this->_smallShip->move(this->_smallShipDirection);
	this->_bigShip->move(this->_bigShipDirection);

	//Move items carried on ships
	this->moveItems(smallShipPiledObjects, this->_smallShipDirection);
	this->moveItems(bigShipPiledObjects, this->_bigShipDirection);

	/*
	Items that fall on each ship are detected thus:
	1. Before advancment of items by gravity - get a list of all of the items that are not blocking the ship from above.
	2. After the advancment: get a list of all of the items that do block the ship from above.
	3. Get a list that is the intersection of the lists from step (1) & step (2).
	*/

	//Advance free-falling items
	this->moveItems(this->_items, DIRECTION_DOWN);

	if(this->_exitPoint->collidesWith(*(this->_smallShip)))
	{
		this->_smallShip->disappear();
		this->_presentShipsCount--;
	}

	if(this->_exitPoint->collidesWith((*(this->_bigShip))))
	{
		this->_bigShip->disappear();
		this->_presentShipsCount--;
	}
}

Game::Game() : _canvas()
{
	this->_exit = false;
	this->_presentShipsCount = 0;
};

bool Game::isGameOver()
{
	return (this->_exit) ||
		(this->_presentShipsCount == 0) ||
		(this->_smallShip->isAlive() == false) ||
		(this->_bigShip->isAlive() == false);
}

void Game::run()
{
	const char * endMessage = "Game OVER!";
	unsigned endMessageEffectIndex = 0;

	while(!(this->_exit))
	{
		this->readUserInput();

		if(this->isGameOver())
		{
			this->_canvas.restore();
			this->_canvas.draw((Canvas::getWidth() - strlen(endMessage)) / 2, Canvas::getHeight() / 2, endMessage);
			this->_canvas.draw((Canvas::getWidth() - strlen(endMessage)) / 2 + endMessageEffectIndex, Canvas::getHeight() / 2, ' ');
			this->_canvas.end();

			endMessageEffectIndex = (endMessageEffectIndex + 1) % strlen(endMessage);
		}
		else
		{
			this->update();
			this->draw_all();

			if(this->isGameOver())
			{
				this->_canvas.save();
			}
		}

		Sleep(50);
	}
}

void Game::addGameObject(Item * item)
{
	this->_items.push_back(item);
	this->_allGameObjects.push_back(item);
	this->_allBlockingObjects.push_back(item);
}

void Game::addGameObject(BigShip * bigShip)
{
	this->_bigShip = bigShip;
	this->_allGameObjects.push_back(bigShip);
	this->_allBlockingObjects.push_back(bigShip);

	this->_presentShipsCount++;
}

void Game::addGameObject(SmallShip * smallShip)
{
	this->_smallShip = smallShip;
	this->_allGameObjects.push_back(smallShip);
	this->_allBlockingObjects.push_back(smallShip);

	this->_presentShipsCount++;
}

void Game::addGameObject(Wall * wall)
{
	this->_walls.push_back(wall);
	this->_allGameObjects.push_back(wall);
	this->_allBlockingObjects.push_back(wall);
}

void Game::addGameObject(ExitPoint * exit)
{
	this->_exitPoint = exit;
	this->_allGameObjects.push_back(exit);
}
