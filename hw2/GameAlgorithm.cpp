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

bool GameAlgorithm::isBlocked(const GameObject & gameObject, const GameObjectSet & blockingObjects, const Point & from, std::vector<const GameObject *> & ignore)
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

void GameAlgorithm::getTouchingObstacles(GameObject & current, const Point & direction, const GameObjectSet & allObstacles, GameObjectSet & touchingObstacles, std::vector<const GameObject *> & ignore)
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

bool GameAlgorithm::isBlocked(const GameObject & gameObject, const GameObjectSet & blockingObjects, const Point & from)
{
	GameObjectSet empty_ignore;

	return GameAlgorithm::isBlocked(gameObject, blockingObjects, from, empty_ignore);
}

bool GameAlgorithm::isBlocked(const GameObject & gameObject, const GameObjectSet & blockingObjects, const Point & from, const GameObjectSet & ignore)
{
	std::vector<const GameObject *> internal_ignore;
	for(GameObjectSet::const_iterator ignoreIter = ignore.cbegin(); ignoreIter != ignore.cend(); ++ignoreIter)
	{
		internal_ignore.push_back(*ignoreIter);
	}

	return GameAlgorithm::isBlocked(gameObject, blockingObjects, from, internal_ignore);
}

bool GameAlgorithm::collidesWith(const GameObject & colider, const GameObjectSet & colidees)
{
	for(GameObjectSet::const_iterator colidee = colidees.cbegin(); colidee != colidees.cend(); ++colidee)
	{
		if(colider.collidesWith(**colidee))
		{
			return true;
		}
	}

	return false;
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
		if(&pileBase != *potentialIter && pileBase.isBlockedBy(**potentialIter, Point::UP))
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

void GameAlgorithm::expandToPushablePile(GameObjectSet & root, const GameObjectSet & obsticales, const Point & pushDirection)
{
	if(pushDirection.notEquals(Point::ZERO))
	{
		GameObjectSet pile(root);
		GameAlgorithm::expandToPile(pile, obsticales);
		GameAlgorithm::removeBlockedFrom(pile, obsticales, pushDirection);

		GameAlgorithm::expandToPile(root, pile);
	}
}

void GameAlgorithm::removeBlockedFrom(GameObjectSet & gameObjects, const GameObjectSet & potentialBlockers, const Point & direction)
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

void GameAlgorithm::getTouchingObstacles(GameObject & root, const Point & direction, const GameObjectSet & allObstacles, GameObjectSet & touchingObstacles)
{
	assert(touchingObstacles.isEmpty());

	std::vector<const GameObject *> ignore;
	GameAlgorithm::getTouchingObstacles(root, direction, allObstacles, touchingObstacles, ignore);
}

void GameAlgorithm::move(GameObjectSet & gameObjects, const Point & direction)
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
		if(ship.isBlockedBy(**itemIter, Point::UP))
		{
			GameObjectSet temp;
			GameAlgorithm::getPiledItems(**itemIter, temp, previouslyFreeFalingItems);
			crashingPile.merge(temp);
			crashingPile.insertUnique(*itemIter);
		}
	}

	const unsigned maxShipEndurance = (ship.getMass() / 2);

	return (crashingPile.getTotalMass() > 0) &&
		(crashingPile.getTotalMass() >= maxShipEndurance);
}

bool GameAlgorithm::isPushDirection(const Point & direction)
{
	return direction.notEquals(Point::ZERO) & direction.notEquals(Point::DOWN);
}