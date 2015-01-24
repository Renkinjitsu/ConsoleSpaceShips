#ifndef GAME_ALGORITHM_H_
#define GAME_ALGORITHM_H_

#include "GameObject.h"
#include "GameObjectSet.h"
#include "Ship.h"

/*
 * Definitions:
 * - Pile - A collection of GameObject-s (usually Item-s) that are touching each other, and each GameObject is placed (resting)
      on another from the smae collection, Execpt for the 'pileBase' which is the bottom of the pile.
 */

class GameAlgorithm
{
private:
	GameAlgorithm();
	~GameAlgorithm();

	static bool isBlocked(const GameObject & gameObject, const GameObjectSet & blockingObjects, const Point & from, std::vector<const GameObject *> & ignore);
	static void getTouchingObstacles(GameObject & current, const Point & direction, const GameObjectSet & allObstacles, GameObjectSet & touchingObstacles, bool recursive, std::vector<const GameObject *> & ignore);
public:

	/*
	 * Checks whether or not there exist any GameObject in 'blockingObjects' that (directly or indirectly)
	 *  blocks 'gameObject' from the direction indicated by 'from'.
	 *
	 * ignore - An optional list of GameObject-s which are skipped if found in 'blockingObjects'.
	 */
	static bool isBlocked(const GameObject & gameObject, const GameObjectSet & blockingObjects, const Point & from);
	static bool isBlocked(const GameObject & gameObject, const GameObjectSet & blockingObjects, const Point & from, const GameObjectSet & ignore);

	static bool collidesWith(const GameObject & colider, const GameObjectSet & colidees);

	/*
	 * Finds & lits the members of a pile that 'pileBase' is it's base.
	 *
	 * 'pile' - An output parameter that holds the pile members.
	 * 'potentialPileMember' - A collection of GameObject-s that are tested as pile members.
	 *
	 * Notes:
	 * - If 'pileBase' is contained in 'pile', the function aborts.
	 *    Otherwise, if 'pile' isn't empty, the consequences are unpredictable.
	 * - The pile returned doesn't contain pileBase.
	 */
	static void getPiledItems(const GameObject & pileBase, GameObjectSet & pile, const GameObjectSet & potentialPileMembers);

	/*
	 * Adds to 'gameObjects' the missing GameObject-s so that for each item in 'gameObjects' it's entire pile shall be contained.
	 *
	 * 'potentialPileMember' - A collection of GameObject-s that are tested as pile members.
	 */
	static void expandToPile(GameObjectSet & gameObjects, const GameObjectSet & potentialPileMembers);

	static void removeBlockedFrom(GameObjectSet & gameObjects, const GameObjectSet & potentialBlockers, const Point & direction);

	/*
	 * Returns a collection of obstacles that touch 'root' from 'direction'.
	 * if 'recursive', adds obstacles that touch others in the collection from 'direction'.
	 */
	static void getTouchingObstacles(GameObject & root, const Point & direction, const GameObjectSet & allObstacles, GameObjectSet & touchingObstacles, bool recursive);

	static bool isTouchingObstacles(const GameObject & gameObject, const GameObjectSet & obstacles, const Point & direction);

	/*
	 * Moves Bombs that should explode & the affected GameObjects to 'detonatedBombs' & 'affectedObjects' respectivly
	 */
	static void handleBombs(GameObjectSet & detonatedBombs, GameObjectSet & affectedObjects, GameObjectSet & bombs, GameObjectSet & potentiallyAffected);

	static void updateBadSpaceshipPosition(GameObject & badShip, const Ship & smallShip, const Ship & bigShip, const GameObjectSet & obstacles);

	/*
	 * Moves the provided game objects 1 step in the provided direction.
	 * No checks of movement validity are performed.
	 */
	static void move(GameObjectSet & gameObjects, const Point & direction);

	static bool isCrashed(GameObject & gameObject, const GameObjectSet & previouslyFreeFalingItems);

	static bool isPushDirection(const Point & direction);
};

#endif /* GAME_ALGORITHM_H_ */