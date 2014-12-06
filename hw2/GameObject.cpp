#include "GameObject.h"

GameObject::GameObject(char texture, bool isPushable)
{
	this->_texture = texture;
	this->_isPushable = isPushable;
}

GameObject::GameObject(char texture, unsigned x, unsigned y, bool isPushable)
{
	this->setLocations(x, y);
	this->_texture = texture;
	this->_isPushable = isPushable;
}

GameObject::GameObject(char texture, const std::vector<Point> & location, bool isPushable)
{
	this->setLocations(location);
	this->_texture = texture;
	this->_isPushable = isPushable;
}

void GameObject::setLocations(unsigned x , unsigned y)
{
	Point point(x, y);
	this->_locations.push_back(point);
}

void GameObject::setLocations(const Point & point)
{
	this->_locations.push_back(point);
}

void GameObject::setLocations(const std::vector<Point> & locations)
{
	for(std::vector<Point>::const_iterator locationIter = locations.cbegin(); locationIter != locations.cend(); ++locationIter)
	{
		this->_locations.push_back(*locationIter);
	}
}

void GameObject::setTexture(const char texture)
{
	this->_texture = texture;
}

void GameObject::clearLocations()
{
	this->_locations.clear();
}

std::vector<Point> GameObject::getLocations()
{
	return this->_locations;
}

bool GameObject::isBlockedBy(const GameObject & other, Direction from) const
{
	for(std::vector<Point>::const_iterator myPosition = this->_locations.begin(); myPosition != this->_locations.end(); ++myPosition)
	{
		Point translatedPosition = *myPosition;
		translatedPosition.move(from);

		for(std::vector<Point>::const_iterator othersPosition = other._locations.begin(); othersPosition != other._locations.end(); ++othersPosition)
		{
			if(translatedPosition.equals(*othersPosition))
			{
				return true;
			}
		}
	}

	return false;
}

bool GameObject::isPushable()
{
	return this->_isPushable;
}

bool GameObject::collidesWith(const GameObject & other) const
{
	for(std::vector<Point>::const_iterator myPosition = this->_locations.begin(); myPosition != this->_locations.end(); ++myPosition)
	{
		for(std::vector<Point>::const_iterator othersPosition = other._locations.begin(); othersPosition != other._locations.end(); ++othersPosition)
		{
			if((*myPosition).equals(*othersPosition))
			{
				return true;
			}
		}
	}

	return false;
}

unsigned GameObject::getMass()
{
	return this->_locations.size();
}

void GameObject::move(Direction direction)
{
	for(std::vector<Point>::iterator locationIter = this->_locations.begin(); locationIter != this->_locations.end(); ++locationIter)
	{
		(*locationIter).move(direction);
	}
}

void GameObject::draw(Canvas & canvas)
{
	for(std::vector<Point>::const_iterator locationIter = this->_locations.begin(); locationIter != this->_locations.end(); ++locationIter)
	{
		canvas.draw((*locationIter).getX(), (*locationIter).getY(), this->_texture);
	}
}
