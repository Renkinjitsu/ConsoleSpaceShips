#include "GameObject.h"
#include "Game.h"
GameObject::GameObject(unsigned x, unsigned y)
{
	this->set_locations(x, y);
}

GameObject::GameObject(vector<Object_location_t> & location)
{
	this->set_locations(location);
}

void GameObject::set_locations(unsigned x , unsigned y)
{
	Object_location_t loc;
	loc.x = x;
	loc.y = y;
	this->locations.push_back(loc);

	start_location.x = x;
	start_location.y = y;

	this->texture_type = SINGLE_LOCATION_TEXTURE;
}


void GameObject::set_locations(vector<Object_location_t> & location)
{
	this->locations = location;

	this->texture_type = MULTI_LOCATION_TEXTURE;
}

unsigned GameObject::getXstart()
{
	// only for SQUARE_TEXTURE
	return this->start_location.x;
}


unsigned GameObject::getYstart()
{
	// only for SQUARE_TEXTURE
	return this->start_location.y;
}
vector<Object_location_t> & GameObject::get_locations()
{
	return this->locations;
}


void GameObject::moveUp()
{
	/*for (size_t i = 0; i < screen.get_num_of_items(); i++)
	{

	}*/


	//this->y--;
}

bool GameObject::moveDown(Game * screen)
{

	//this->y++;
	return true;
}

void GameObject::moveLeft()
{
	//this->x--;
}

void GameObject::moveRight()
{
	//this->y++;
}
