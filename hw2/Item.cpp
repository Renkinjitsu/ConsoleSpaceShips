#include "Definitions.h"
#include "Item.h"
#include "config.h"

Item::Item(int item_number, const Object_location_t locations_array[], size_t locations_array_len) : GameObject(OBJECT_ITEM)
{
	this->item_number = item_number;

	vector<Object_location_t> locations;
	for (size_t i = 0; i < locations_array_len; i++)
	{
		locations.push_back(locations_array[i]);
	}

	set_locations(locations);
}

bool Item::canMoveDown(Game * game_screen, game_move_flags_t flags)
{
	return game_screen->canMoveX((GameObject *)this, DIRECTION_DOWN, flags);
}

bool Item::canMoveUp(Game * game_screen, game_move_flags_t flags)
{
	return game_screen->canMoveX((GameObject *)this, DIRECTION_UP, flags);
}

bool Item::canMoveLeft(Game * game_screen, game_move_flags_t flags)
{
	return game_screen->canMoveX((GameObject *)this, DIRECTION_LEFT, flags);
}

bool Item::canMoveRight(Game * game_screen, game_move_flags_t flags)
{
	return game_screen->canMoveX((GameObject *)this, DIRECTION_RIGHT, flags);
}

bool Item::canItemMoveX(Game * game_screen, game_direction_e direction, game_move_flags_t flags)
{
	return game_screen->canMoveX((GameObject *)this, direction, flags);
}

bool Item::MoveX(Game * game_screen, game_direction_e direction, game_move_flags_t flags)
{
	switch (direction)
	{
	case DIRECTION_UP:
	{
		return MoveUp(game_screen, flags);
	}
		break;

	case DIRECTION_DOWN:
	{
		return MoveDown(game_screen, flags);
	}
		break;

	case DIRECTION_RIGHT:
	{
		return MoveRight(game_screen, flags);
	}
		break;

	case DIRECTION_LEFT:
	{
		return MoveLeft(game_screen, flags);
	}
		break;
	}

	return false;
}


bool Item::MoveUp(Game * game_screen, game_move_flags_t flags)
{
	//Lets go up!
	if (this->canMoveUp(game_screen, flags))
	{
		EraseDrawing(game_screen->getCanvas());
		GameObject::moveUp();

		draw(game_screen->getCanvas());

		return true;
	}
	return false;
}


bool Item::MoveDown(Game * game_screen, game_move_flags_t flags)
{
	//Lets go down!
	if (this->canMoveDown(game_screen, flags))
	{
		EraseDrawing(game_screen->getCanvas());
		GameObject::moveDown();


		//TODO: check for explosion if flags.explosion_allowed
		if (flags.explosion_allowed)
		{

		}

		draw(game_screen->getCanvas());

		return true;
	}
	return false;
}

bool Item::MoveLeft(Game * game_screen, game_move_flags_t flags)
{
	//Lets go down!
	if (this->canMoveLeft(game_screen, flags))
	{
		EraseDrawing(game_screen->getCanvas());
		GameObject::moveLeft();
		draw(game_screen->getCanvas());

		return true;
	}
	return false;
}

bool Item::MoveRight(Game * game_screen, game_move_flags_t flags)
{
	//Lets go down!
	if (this->canMoveRight(game_screen, flags))
	{
		EraseDrawing(game_screen->getCanvas());
		GameObject::moveRight();
		draw(game_screen->getCanvas());

		return true;
	}
	return false;
}