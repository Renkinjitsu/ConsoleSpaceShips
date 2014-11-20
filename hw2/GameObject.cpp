#include "GameObject.h"
#include "Game.h"

GameObject::GameObject(unsigned x, unsigned y)
{
	this->x = x;
	this->y = y;
}

unsigned GameObject::getX()
{
	return this->x;
}

unsigned GameObject::getY()
{
	return this->y;
}

void GameObject::setPosition(unsigned x, unsigned y)
{
	this->x = x;
	this->y = y;
}

void GameObject::moveUp()
{
	/*for (size_t i = 0; i < screen.get_num_of_items(); i++)
	{

	}*/

	this->y--;
}

void GameObject::moveDown()
{
	this->y++;
}

void GameObject::moveLeft()
{
	this->x--;
}

void GameObject::moveRight()
{
	this->y++;
}
