#include "GameAlgorithm.h"

#include <assert.h>

GameAlgorithm::GameAlgorithm()
{
	//Nothing to do
}


GameAlgorithm::~GameAlgorithm()
{
	//Nothing to do
}

bool GameAlgorithm::isBlocked(const GameObject & gameObject, const GameObjectSet & blockingObjects, Direction from, std::vector<const GameObject *> & ignore)
{
	if(std::find(ignore.begin(), ignore.end(), &gameObject) != ignore.end())
	{
		return false;
	}

	ignore.push_back(&gameObject);

	for(GameObjectSet::const_iterator blockingObjectIter = blockingObjects.cbegin(); blockingObjectIter != blockingObjects.cend(); ++blockingObjectIter)
	{
		if(*blockingObjectIter != &gameObject &&
			gameObject.isBlockedBy(**blockingObjectIter, from) &&
			(std::find(ignore.begin(), ignore.end(), *blockingObjectIter) == ignore.end()))
		{
			if((*blockingObjectIter)->isPushable() == false ||
				GameAlgorithm::isBlocked(**blockingObjectIter, blockingObjects, from, ignore))
			{
				return true;
			}
		}
	}

	return false;
}

void GameAlgorithm::getTouchingObstacles(GameObject & current, Direction direction, const GameObjectSet & allObstacles, GameObjectSet & touchingObstacles, std::vector<const GameObject *> & ignore)
{
	ignore.push_back(&current);

	for(GameObjectSet::const_iterator itemIter = allObstacles.cbegin(); itemIter != allObstacles.cend(); ++itemIter)
	{
		if(std::find(ignore.begin(), ignore.end(), *itemIter) != ignore.end()) //Already checked
		{
			//Skip
		}
		else if(current.isBlockedBy(**itemIter, direction))
		{
			touchingObstacles.insert(*itemIter);
			GameAlgorithm::getTouchingObstacles(**itemIter, direction, allObstacles, touchingObstacles, ignore);
		}
	}
}

bool GameAlgorithm::isBlocked(const GameObject & gameObject, const GameObjectSet & blockingObjects, Direction from)
{
	GameObjectSet empty_ignore;

	return GameAlgorithm::isBlocked(gameObject, blockingObjects, from, empty_ignore);
}

bool GameAlgorithm::isBlocked(const GameObject & gameObject, const GameObjectSet & blockingObjects, Direction from, const GameObjectSet & ignore)
{
	std::vector<const GameObject *> internal_ignore;
	for(GameObjectSet::const_iterator ignoreIter = ignore.cbegin(); ignoreIter != ignore.cend(); ++ignoreIter)
	{
		internal_ignore.push_back(*ignoreIter);
	}

	return GameAlgorithm::isBlocked(gameObject, blockingObjects, from, internal_ignore);
}

void GameAlgorithm::getPiledItems(const GameObject & pileBase, GameObjectSet & pile, const GameObjectSet & potentialPileMembers)
{
	if(pile.contains(&pileBase))
	{
		return;
	}

	for(GameObjectSet::const_iterator potentialIter = potentialPileMembers.cbegin();
		potentialIter != potentialPileMembers.cend(); ++potentialIter)
	{
		if(&pileBase != *potentialIter && pileBase.isBlockedBy(**potentialIter, DIRECTION_UP))
		{
			pile.insertUnique(*potentialIter);
			GameAlgorithm::getPiledItems(**potentialIter, pile, potentialPileMembers);
		}
	}
}

void GameAlgorithm::expandToPile(GameObjectSet & gameObjects, const GameObjectSet & potentialPileMembers)
{
	GameObjectSet piles;

	for(GameObjectSet::const_iterator gameObjectIter = gameObjects.cbegin();
		gameObjectIter != gameObjects.cend(); ++gameObjectIter)
	{
		GameAlgorithm::getPiledItems(**gameObjectIter, piles, potentialPileMembers);
	}

	gameObjects.merge(piles);
}

void GameAlgorithm::expandToPushablePile(GameObjectSet & root, const GameObjectSet & obsticales, Direction pushDirection)
{
	GameObjectSet pile(root);
	GameAlgorithm::expandToPile(pile, obsticales);
	GameAlgorithm::removeBlockedFrom(pile, obsticales, pushDirection);

	GameAlgorithm::expandToPile(root, pile);
}

void GameAlgorithm::removeBlockedFrom(GameObjectSet & gameObjects, const GameObjectSet & potentialBlockers, Direction direction)
{
	GameObjectSet::iterator blockedIter = gameObjects.begin();
	while(blockedIter != gameObjects.end())
	{
		if((*blockedIter)->isPushable() == false ||
			GameAlgorithm::isBlocked(**blockedIter, potentialBlockers, direction))
		{
			blockedIter = gameObjects.remove(blockedIter);
		}
		else
		{
			++blockedIter;
		}
	}
}

void GameAlgorithm::getTouchingObstacles(GameObject & root, Direction direction, const GameObjectSet & allObstacles, GameObjectSet & touchingObstacles)
{
	assert(touchingObstacles.isEmpty());

	std::vector<const GameObject *> ignore;
	GameAlgorithm::getTouchingObstacles(root, direction, allObstacles, touchingObstacles, ignore);
}

void GameAlgorithm::move(GameObjectSet & gameObjects, Direction direction)
{
	for(GameObjectSet::iterator iter = gameObjects.begin(); iter != gameObjects.end(); ++iter)
	{
		(*iter)->move(direction);
	}
}

bool GameAlgorithm::isShipCrashed(const Ship & ship, const GameObjectSet & previouslyFreeFalingItems)
{
	GameObjectSet crashingPile;

	for(GameObjectSet::const_iterator itemIter = previouslyFreeFalingItems.cbegin();
		itemIter != previouslyFreeFalingItems.cend(); ++itemIter)
	{
		if(ship.isBlockedBy(**itemIter, DIRECTION_UP))
		{
			GameObjectSet temp;
			GameAlgorithm::getPiledItems(**itemIter, temp, previouslyFreeFalingItems);
			crashingPile.merge(temp);
			crashingPile.insertUnique(*itemIter);
		}
	}

	return crashingPile.getTotalMass() >= (ship.getMass() / 2);
}

bool GameAlgorithm::isPushDirection(Direction direction)
{
	return (direction == DIRECTION_LEFT) || (direction == DIRECTION_RIGHT) || (direction == DIRECTION_UP);
}