#include "GameObject.h"

GameObject::GameObject(char texture, bool isPushable)
{
	this->_texture = texture;
	this->_isPushable = isPushable;
}

GameObject::GameObject(char texture, const std::vector<Point> & points, bool isPushable)
{
	this->setPoints(points);
	this->_texture = texture;
	this->_isPushable = isPushable;
}

void GameObject::setPoints(unsigned x, unsigned y)
{
	Point point(x, y);
	this->_points.push_back(point);
}

void GameObject::setPoints(const Point & point)
{
	this->_points.push_back(point);
}

void GameObject::setPoints(const std::vector<Point> & points)
{
	for(unsigned i = 0; i < points.size(); ++i)
	{
		this->_points.push_back(points[i]);
	}
}

char GameObject::getTexture() const
{
	return this->_texture;
}

void GameObject::setTexture(const char texture)
{
	this->_texture = texture;
}

void GameObject::clearPoints()
{
	this->_points.clear();
}

std::vector<Point> GameObject::getPoints() const
{
	return this->_points;
}

bool GameObject::isBlockedBy(const GameObject & other, const Point & fromDirection) const
{
	for(unsigned i = 0; i < this->_points.size(); ++i)
	{
		Point translatedPosition = this->_points[i];
		translatedPosition.move(fromDirection);

		for(unsigned j = 0; j < other._points.size(); ++j)
		{
			if(translatedPosition.equals(other._points[j]))
			{
				return true;
			}
		}
	}

	return false;
}

bool GameObject::isPushable() const
{
	return this->_isPushable;
}

bool GameObject::collidesWith(const GameObject & other) const
{
	for(unsigned i = 0; i < this->_points.size(); ++i)
	{
		for(unsigned j = 0; j < other._points.size(); ++j)
		{
			if(this->_points[i].equals(other._points[j]))
			{
				return true;
			}
		}
	}

	return false;
}

unsigned GameObject::getMass() const
{
	return this->_points.size();
}

void GameObject::move(const Point & offset)
{
	for(unsigned i = 0; i < this->_points.size(); ++i)
	{
		this->_points[i].move(offset);
	}
}

void GameObject::draw(Canvas & canvas)
{
	for(unsigned i = 0; i < this->_points.size(); ++i)
	{
		canvas.draw(this->_points[i], this->_texture);
	}
}
