#include "GameObject.h"
#include "Game.h"
GameObject::GameObject(unsigned x, unsigned y)
{
	this->set_locations(x, y);
}

GameObject::GameObject(vector<Item_location_t> & location)
{
	this->set_locations(location);
}

void GameObject::set_locations(unsigned x , unsigned y)
{
	Item_location_t loc;
	loc.x = x;
	loc.y = y;
	this->locations.push_back(loc);

	start_location.x = x;
	start_location.y = y;

	this->texture_type = SQUARE_TEXTURE;
}


void GameObject::set_locations(vector<Item_location_t> & location)
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
vector<Item_location_t> & GameObject::get_locations()
{
	return this->locations;
}


bool GameObject::IsUpperTouchingDowner(GameObject * downer, GameObject * upper)
{
	//if (downer->x)
	return false;
}

/*void GameObject::setPosition(unsigned x, unsigned y)
{
	this->x = x;
	this->y = y;
}*/

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
