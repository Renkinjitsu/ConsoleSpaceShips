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

GameObjectSet & GameObjectSet::operator+=(GameObject * const gameObject)
{
	if(this->contains(gameObject) == false)
	{
		_collection.push_back(gameObject);
	}

	return *this;
}

GameObjectSet & GameObjectSet::operator+=(const GameObjectSet & other)
{
	for(GameObjectSet::const_iterator iter = other._collection.begin();
		iter != other._collection.end(); ++iter)
	{
		(*this) += (*iter);
	}

	return *this;
}

GameObjectSet & GameObjectSet::operator-=(const GameObject * const gameObject)
{
	GameObjectSet::iterator iter =
		std::find(_collection.begin(), _collection.end(), gameObject);

	if(iter != _collection.end())
	{
		_collection.erase(iter);
		assert(this->contains(gameObject) == false);
	}

	return *this;
}

GameObjectSet & GameObjectSet::operator-=(const GameObjectSet & other)
{
	for(GameObjectSet::const_iterator iter = other.cbegin();
		iter != other.cend(); ++iter)
	{
		(*this) -= *iter;
	}

	return *this;
}

bool GameObjectSet::operator==(const GameObjectSet & other) const
{
	bool result = (_collection.size() == other._collection.size());

	for(GameObjectSet::const_iterator iter = other._collection.begin();
		iter != other._collection.end() && result; ++iter)
	{
		result &= this->contains(*iter);
	}

	return result;
}

bool GameObjectSet::operator!=(const GameObjectSet & other) const
{
	return !((*this) == other);
}

bool GameObjectSet::contains(const GameObject * const gameObject) const
{
	return std::find(_collection.begin(), _collection.end(), gameObject) != _collection.end();
}

bool GameObjectSet::isEmpty() const
{
	return _collection.empty();
}

void GameObjectSet::clear()
{
	_collection.clear();
}

bool GameObjectSet::isPushable() const
{
	bool isPushable = true;

	for(GameObjectSet::const_iterator iter = _collection.begin();
		iter != _collection.end() && isPushable; ++iter)
	{
		isPushable &= (*iter)->isPushable();
	}

	return isPushable;
}

unsigned GameObjectSet::getTotalMass() const
{
	unsigned totalMass = 0;

	for(GameObjectSet::const_iterator iter = _collection.begin();
		iter != _collection.end(); ++iter)
	{
		totalMass += (*iter)->getMass();
	}

	return totalMass;
}

GameObjectSet::iterator GameObjectSet::begin()
{
	return _collection.begin();
}

GameObjectSet::const_iterator GameObjectSet::cbegin() const
{
	return _collection.cbegin();
}

GameObjectSet::iterator GameObjectSet::end()
{
	return _collection.end();
}

GameObjectSet::const_iterator GameObjectSet::cend() const
{
	return _collection.cend();
}

GameObjectSet::iterator GameObjectSet::erase(const GameObjectSet::iterator & gameObjectIter)
{
	return _collection.erase(gameObjectIter);
}
