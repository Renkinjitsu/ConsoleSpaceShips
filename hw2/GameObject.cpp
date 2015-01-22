#include "GameObject.h"

GameObject::GameObject(char texture, bool isPushable)
{
	_texture = texture;
	_isPushable = isPushable;
}

GameObject::GameObject(char texture, const std::vector<Point> & points, bool isPushable)
{
	this->setPoints(points);
	_texture = texture;
	_isPushable = isPushable;
}

void GameObject::setPoints(unsigned x, unsigned y)
{
	Point point(x, y);
	_points.push_back(point);
}

void GameObject::setPoints(const Point & point)
{
	_points.push_back(point);
}

void GameObject::setPoints(const std::vector<Point> & points)
{
	for(unsigned i = 0; i < points.size(); ++i)
	{
		_points.push_back(points[i]);
	}
}

char GameObject::getTexture() const
{
	return _texture;
}

void GameObject::setTexture(const char texture)
{
	_texture = texture;
}

void GameObject::clearPoints()
{
	_points.clear();
}

std::vector<Point> GameObject::getPoints() const
{
	return _points;
}

bool GameObject::isBlockedBy(const GameObject & other, const Point & fromDirection) const
{
	for(unsigned i = 0; i < _points.size(); ++i)
	{
		const Point translatedPosition = _points[i] + fromDirection;

		for(unsigned j = 0; j < other._points.size(); ++j)
		{
			if(translatedPosition == other._points[j])
			{
				return true;
			}
		}
	}

	return false;
}

bool GameObject::isPushable() const
{
	return _isPushable;
}

bool GameObject::collidesWith(const GameObject & other) const
{
	for(unsigned i = 0; i < _points.size(); ++i)
	{
		for(unsigned j = 0; j < other._points.size(); ++j)
		{
			if(_points[i] == other._points[j])
			{
				return true;
			}
		}
	}

	return false;
}

bool GameObject::isTouching(const GameObject & other) const
{
	return this->isBlockedBy(other, Point::LEFT) ||
		this->isBlockedBy(other, Point::RIGHT) ||
		this->isBlockedBy(other, Point::UP) ||
		this->isBlockedBy(other, Point::DOWN);
}

unsigned GameObject::getMass() const
{
	return _points.size();
}

Point GameObject::getTopLeft() const
{
	unsigned top = this->_points[0].getY();
	unsigned left = this->_points[0].getX();

	for(unsigned i = 1; i < this->_points.size(); ++i)
	{
		top = Point::getTop(top, this->_points[i].getY());
		left = Point::getLeft(left, this->_points[i].getX());
	}

	return Point(left, top);
}

void GameObject::move(const Point & offset)
{
	for(unsigned i = 0; i < _points.size(); ++i)
	{
		_points[i] += offset;
	}
}

void GameObject::draw(Canvas & canvas)
{
	for(unsigned i = 0; i < _points.size(); ++i)
	{
		canvas.draw(_points[i], _texture);
	}
}
