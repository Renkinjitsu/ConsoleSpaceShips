#include "GameObjectSet.h"

#include <assert.h>

GameObjectSet::GameObjectSet()
{
	//Do nothing
}

GameObjectSet::GameObjectSet(const GameObjectSet & other) : _collection(other._collection.cbegin(), other._collection.cend())
{
	//Do nothing
}

GameObjectSet::~GameObjectSet()
{
	//Do nothing
}

void GameObjectSet::insert(GameObject * gameObject)
{
	assert(this->contains(gameObject) == false);

	this->_collection.push_back(gameObject);
}

void GameObjectSet::insertUnique(GameObject * gameObject)
{
	if(this->contains(gameObject) == false)
	{
		this->insert(gameObject);
	}
}

void GameObjectSet::merge(const GameObjectSet & other)
{
	for(std::vector<GameObject *>::const_iterator iter = other._collection.begin();
		iter != other._collection.end(); ++iter)
	{
		this->insertUnique(*iter);
	}
}

void GameObjectSet::remove(const GameObject * const gameObject)
{
	GameObjectSet::iterator iter =
		std::find(this->_collection.begin(), this->_collection.end(), gameObject);

	if(iter != this->_collection.end())
	{
		this->_collection.erase(iter);
		assert(this->contains(gameObject) == false);
	}
}

GameObjectSet::iterator GameObjectSet::remove(const GameObjectSet::iterator & gameObjectIter)
{
	const GameObject * const gameObject = *gameObjectIter;

	GameObjectSet::iterator next = this->_collection.erase(gameObjectIter);
	assert(this->contains(gameObject) == false);

	return next;
}

void GameObjectSet::remove(const GameObjectSet & other)
{
	for(std::vector<GameObject *>::const_iterator iter = other._collection.begin();
		iter != other._collection.end(); ++iter)
	{
		this->remove(*iter);
	}
}

void GameObjectSet::clear()
{
	this->_collection.clear();
}

bool GameObjectSet::isEqual(const GameObjectSet & other) const
{
	bool result = (this->_collection.size() == other._collection.size());

	for(std::vector<GameObject *>::const_iterator iter = other._collection.begin();
		iter != other._collection.end() && result; ++iter)
	{
		result &= this->contains(*iter);
	}

	return result;
}

bool GameObjectSet::contains(const GameObject * const gameObject) const
{
	return std::find(this->_collection.begin(), this->_collection.end(), gameObject) != this->_collection.end();
}

bool GameObjectSet::isEmpty() const
{
	return this->_collection.empty();
}

bool GameObjectSet::isPushable() const
{
	bool isPushable = true;

	for(GameObjectSet::const_iterator iter = this->_collection.begin();
		iter != this->_collection.end() && isPushable; ++iter)
	{
		isPushable &= (*iter)->isPushable();
	}

	return isPushable;
}

unsigned GameObjectSet::getTotalMass() const
{
	unsigned totalMass = 0;

	for(GameObjectSet::const_iterator iter = this->_collection.begin();
		iter != this->_collection.end(); ++iter)
	{
		totalMass += (*iter)->getMass();
	}

	return totalMass;
}

GameObjectSet::iterator GameObjectSet::begin()
{
	return this->_collection.begin();
}

GameObjectSet::const_iterator GameObjectSet::cbegin() const
{
	return this->_collection.cbegin();
}

GameObjectSet::iterator GameObjectSet::end()
{
	return this->_collection.end();
}

GameObjectSet::const_iterator GameObjectSet::cend() const
{
	return this->_collection.cend();
}
