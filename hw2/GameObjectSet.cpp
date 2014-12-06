#include "GameObjectSet.h"

#include <assert.h>

GameObjectSet::GameObjectSet()
{
}


GameObjectSet::~GameObjectSet()
{
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

void GameObjectSet::remove(const GameObject * const gameObject)
{
	GameObjectSet::iterator iter = this->_collection.begin();

	while(iter != this->_collection.end())
	{
		if(*iter == gameObject)
		{
			this->_collection.erase(iter);
			iter = this->_collection.begin();
		}
		else
		{
			++iter;
		}
	}
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
