#ifndef GAME_OBJECT_SET_H_
#define GAME_OBJECT_SET_H_

#include <vector>
#include <iterator>

#include "GameObject.h"

class GameObjectSet
{
	std::vector<GameObject *> _collection;
public:
	GameObjectSet();
	GameObjectSet(const GameObjectSet & other);
	~GameObjectSet();

	void insert(GameObject * gameObject);
	void insertUnique(GameObject * gameObject);
	void merge(const GameObjectSet & other);

	void remove(const GameObject * const gameObject);
	void remove(const GameObjectSet & other);

	void clear();

	bool isEqual(const GameObjectSet & other) const;
	bool contains(const GameObject * const gameObject) const;
	bool isEmpty() const;

	bool isPushable() const;
	unsigned getTotalMass() const;

	//Iterator:

	typedef std::vector<GameObject *>::iterator iterator;
	typedef std::vector<GameObject *>::const_iterator const_iterator;

	GameObjectSet::iterator begin();
	GameObjectSet::const_iterator cbegin() const;

	GameObjectSet::iterator end();
	GameObjectSet::const_iterator cend() const;

	GameObjectSet::iterator remove(const GameObjectSet::iterator & gameObjectIter);
};

#endif /* GAME_OBJECT_SET_H_ */
