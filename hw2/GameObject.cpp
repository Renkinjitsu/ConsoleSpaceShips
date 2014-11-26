#include "GameObject.h"
#include "Game.h"
GameObject::GameObject(Object_type_e object_type, unsigned x, unsigned y)
{
	this->set_locations(x, y);
	this->object_type = object_type;
}

GameObject::GameObject(vector<Object_location_t> & location, Object_type_e object_type)
{
	this->set_locations(location);
	this->object_type = object_type;
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

void GameObject::clear_locations()
{
	this->locations.clear();
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

Object_location_t GameObject::get_location()
{
	return this->start_location;
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
