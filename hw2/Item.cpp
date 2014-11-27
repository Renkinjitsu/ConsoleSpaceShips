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

bool Item::canMoveDown(Game * game_screen)
{
	return game_screen->canMoveX((GameObject *)this, DIRECTION_DOWN);
}

bool Item::canMoveUp(Game * game_screen)
{
	return game_screen->canMoveX((GameObject *)this, DIRECTION_UP);
}

bool Item::canMoveLeft(Game * game_screen)
{
	return game_screen->canMoveX((GameObject *)this, DIRECTION_LEFT);
}

bool Item::canMoveRight(Game * game_screen)
{
	return game_screen->canMoveX((GameObject *)this, DIRECTION_RIGHT);
}

bool Item::canItemMoveX(Game * game_screen, Direction direction)
{
	return game_screen->canMoveX((GameObject *)this, direction);
}

bool Item::MoveX(Game * game_screen, Direction direction)
{
	switch (direction)
	{
		case DIRECTION_UP:
		{
			return MoveUp(game_screen);
		}
		break;

		case DIRECTION_DOWN:
		{
			return MoveDown(game_screen);
		}
		break;

		case DIRECTION_RIGHT:
		{
			return MoveRight(game_screen);
		}
		break;

		case DIRECTION_LEFT:
		{
			return MoveLeft(game_screen);
		}
		break;
	}

	return false;
}


bool Item::MoveUp(Game * game_screen)
{
	//Lets go up!
	if (this->canMoveUp(game_screen))
	{
		EraseDrawing(game_screen->getCanvas());
		vector<Object_location_t> & locations = get_locations();
		for (vector<Object_location_t>::iterator it = locations.begin(); it != locations.end(); ++it)
		{
			if (it->y == 0)
			{
				it->y = Canvas::getHeight();
			}
			else
			{
				it->y--;
			}
		}

		draw(game_screen->getCanvas());

		return true;
	}
	return false;
}


bool Item::MoveDown(Game * game_screen)
{
	//Lets go down!
	if (this->canMoveDown(game_screen))
	{
		EraseDrawing(game_screen->getCanvas());
		vector<Object_location_t> & locations = get_locations();
		for (vector<Object_location_t>::iterator it = locations.begin(); it != locations.end(); ++it)
		{
			if (it->y == Canvas::getHeight())
			{
				it->y = it->y % Canvas::getHeight();
			}
			else
			{
				it->y++;
			}
		}

		draw(game_screen->getCanvas());

		return true;
	}
	return false;
}

bool Item::MoveLeft(Game * game_screen)
{
	//Lets go down!
	if (this->canMoveLeft(game_screen))
	{
		EraseDrawing(game_screen->getCanvas());
		vector<Object_location_t> & locations = get_locations();
		for (vector<Object_location_t>::iterator it = locations.begin(); it != locations.end(); ++it)
		{
			if (it->x == 0)
			{
				it->x = Canvas::getWidth();
			}
			else
			{
				it->x--;
			}
		}

		draw(game_screen->getCanvas());

		return true;
	}
	return false;
}

bool Item::MoveRight(Game * game_screen)
{
	//Lets go down!
	if (this->canMoveRight(game_screen))
	{
		EraseDrawing(game_screen->getCanvas());
		vector<Object_location_t> & locations = get_locations();
		for (vector<Object_location_t>::iterator it = locations.begin(); it != locations.end(); ++it)
		{
			if (it->x == Canvas::getWidth())
			{
				it->x = it->x % Canvas::getWidth();
			}
			else
			{
				it->x++;
			}
		}

		draw(game_screen->getCanvas());

		return true;
	}
	return false;
}