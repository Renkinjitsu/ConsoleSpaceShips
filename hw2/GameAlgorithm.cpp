#include "GameAlgorithm.h"

#include <assert.h>
#include <algorithm>

#include "GameObject.h"
#include "GameObjectSet.h"
#include "Bomb.h"
#include "Area.h"
#include "Item.h"

bool GameAlgorithm::isBlocked(const GameObject & gameObject, const GameObjectSet & blockingObjects, const Point & from, std::vector<const GameObject *> & ignore)
{
	if(std::find(ignore.begin(), ignore.end(), &gameObject) != ignore.end())
	{
		return false;
	}

	ignore.push_back(&gameObject);

	for(GameObjectSet::const_iterator blockingObjectIter = blockingObjects.cbegin(); blockingObjectIter != blockingObjects.cend(); ++blockingObjectIter)
	{
		const GameObject & blockingObject = **blockingObjectIter;

		if(gameObject.isBlockedBy(blockingObject, from))
		{
			const bool ignored = std::find(ignore.begin(), ignore.end(), &blockingObject) != ignore.end();
			if(ignored == false)
			{
				if(blockingObject.isPushable() == false || GameAlgorithm::isBlocked(blockingObject, blockingObjects, from, ignore))
				{
					return true;
				}
			}
		}
	}

	return false;
}

void GameAlgorithm::getTouchingObstacles(GameObject & current, const Point & direction, const GameObjectSet & allObstacles, GameObjectSet & touchingObstacles, bool recursive, std::vector<const GameObject *> & ignore)
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
			touchingObstacles += (*itemIter);

			if(recursive)
			{
				GameAlgorithm::getTouchingObstacles(**itemIter, direction, allObstacles, touchingObstacles, recursive, ignore);
			}
		}
	}
}

bool GameAlgorithm::isBlocked(const GameObject & gameObject, const GameObjectSet & blockingObjects, const Point & from)
{
	const GameObjectSet empty_ignore;

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
			pile += (*potentialIter);
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

	gameObjects += piles;
}

void GameAlgorithm::removeBlockedFrom(GameObjectSet & gameObjects, const GameObjectSet & potentialBlockers, const Point & direction)
{
	GameObjectSet::iterator blockedIter = gameObjects.begin();
	while(blockedIter != gameObjects.end())
	{
		if((*blockedIter)->isPushable() == false ||
			GameAlgorithm::isBlocked(**blockedIter, potentialBlockers, direction))
		{
			blockedIter = gameObjects.erase(blockedIter);
		}
		else
		{
			++blockedIter;
		}
	}
}

void GameAlgorithm::getTouchingObstacles(GameObject & root, const Point & direction, const GameObjectSet & allObstacles, GameObjectSet & touchingObstacles, bool recursive)
{
	std::vector<const GameObject *> ignore;
	GameAlgorithm::getTouchingObstacles(root, direction, allObstacles, touchingObstacles, recursive, ignore);
}

bool GameAlgorithm::isTouchingObstacles(const GameObject & gameObject, const GameObjectSet & obstacles, const Point & direction)
{
	bool result = false;

	for(GameObjectSet::const_iterator obstacleIter = obstacles.cbegin(); (obstacleIter != obstacles.cend()) && (result == false); ++obstacleIter)
	{
		result = gameObject.isBlockedBy(**obstacleIter, direction);
	}

	return result;
}

void GameAlgorithm::handleBombs(GameObjectSet & detonatedBombs, GameObjectSet & affectedObjects, GameObjectSet & bombs, GameObjectSet & potentiallyAffected)
{
	//Look for detonated bombs
	for(GameObjectSet::iterator bombIter = bombs.begin();
		bombIter != bombs.end();
		++bombIter)
	{
		for(GameObjectSet::const_iterator potentialIter = potentiallyAffected.cbegin();
			potentialIter != potentiallyAffected.cend();
			++potentialIter)
		{
			if((*bombIter)->isTouching(**potentialIter))
			{
				detonatedBombs += (*bombIter);
			}
		}
	}

	const unsigned explosionLength = (Bomb::getExplosionDistance() * 2) + 1;

	//For each detonated bomb, list effects
	for(GameObjectSet::const_iterator detonatedIter = detonatedBombs.cbegin();
		detonatedIter != detonatedBombs.cend();
		++detonatedIter)
	{
		//Prepare explosion area object
		const Point areaTopLeft = (*detonatedIter)->getTopLeft() + ((Point::UP + Point::LEFT) * Bomb::getExplosionDistance());
		const Area explosionArea(areaTopLeft, explosionLength, explosionLength);

		for(GameObjectSet::iterator potentialIter = potentiallyAffected.begin();
			potentialIter != potentiallyAffected.end();
			++potentialIter)
		{
			if(explosionArea.collidesWith(**potentialIter))
			{
				if(bombs.contains(*potentialIter))
				{
					detonatedBombs += (*potentialIter);
				}
				else
				{
					affectedObjects += (*potentialIter);
				}
			}
		}
	}
}

void GameAlgorithm::updateBadSpaceshipPosition(GameObject & badShip, const Ship & smallShip, const Ship & bigShip, const GameObjectSet & obstacles)
{
	const Point badShipPosition = badShip.getTopLeft();

	const Point smallShipClosestPoint = smallShip.getClosestStepDistancePoint(badShip);
	const Point bigShipClosestPoint = bigShip.getClosestStepDistancePoint(badShip);

	const unsigned smallShipDistance = badShip.getStepDistance(smallShipClosestPoint);
	const unsigned bigShipDistance = badShip.getStepDistance(bigShipClosestPoint);

	const Point & targetPoint = (bigShipDistance <= smallShipDistance) ? bigShipClosestPoint : smallShipClosestPoint;

	Point verticalMovement = Point::ZERO;
	if(targetPoint.isAbove(badShipPosition))
	{
		if(GameAlgorithm::isTouchingObstacles(badShip, obstacles, Point::UP) == false)
		{
			verticalMovement = Point::UP;
		}
	}
	else if(targetPoint.isBelow(badShipPosition))
	{
		if(GameAlgorithm::isTouchingObstacles(badShip, obstacles, Point::DOWN) == false)
		{
			verticalMovement = Point::DOWN;
		}
	}
	//else, no horizontal movement

	Point horisontalMovement = Point::ZERO;
	if(targetPoint.isLeftOf(badShipPosition))
	{
		if(GameAlgorithm::isTouchingObstacles(badShip, obstacles, Point::LEFT) == false)
		{
			horisontalMovement = Point::LEFT;
		}
	}
	else if(targetPoint.isRightOf(badShipPosition))
	{
		if(GameAlgorithm::isTouchingObstacles(badShip, obstacles, Point::RIGHT) == false)
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

void GameAlgorithm::move(GameObjectSet & gameObjects, const Point & direction)
{
	for(GameObjectSet::iterator iter = gameObjects.begin(); iter != gameObjects.end(); ++iter)
	{
		(*iter)->move(direction);
	}
}

bool GameAlgorithm::isCrashed(GameObject & gameObject, const GameObjectSet & previouslyFreeFalingItems)
{
	GameObjectSet crashingPile;
	GameAlgorithm::getTouchingObstacles(gameObject, Point::UP, previouslyFreeFalingItems, crashingPile, true);

	const unsigned maxEndurance = gameObject.getMass() / 2;
	const unsigned crashingPileTotalMass = crashingPile.getTotalMass();

	return (crashingPileTotalMass > 0) && (crashingPileTotalMass >= maxEndurance);
}

bool GameAlgorithm::isPushDirection(const Point & direction)
{
	return (direction != Point::ZERO) && (direction != Point::DOWN);
}
