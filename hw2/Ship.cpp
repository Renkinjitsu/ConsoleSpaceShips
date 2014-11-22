#include "Ship.h"
#include "Game.h"

Ship::Ship(unsigned xPositin, unsigned yPosition,
	unsigned width, unsigned height,
	char texture) : GameObject(xPositin, yPosition), texture(texture)
{
	this->width = width;
	this->height = height;

	vector<Object_location_t> locations;

	for (size_t i = 0; i < width; i++)
	{
		for (size_t j = 0; j < height; j++)
		{
			Object_location_t location;
			location.x = xPositin + i;
			location.y = yPosition + j;

			locations.push_back(location);
		}
	}

	set_locations(locations);
}

bool Ship::canMoveDown(Game * game_screen)
{
	return game_screen->canMoveX((GameObject *)this, DIRECTION_DOWN);
}

bool Ship::canMoveUp(Game * game_screen)
{
	return game_screen->canMoveX((GameObject *)this, DIRECTION_UP);
}

bool Ship::canMoveLeft(Game * game_screen)
{
	return game_screen->canMoveX((GameObject *)this, DIRECTION_LEFT);
}

bool Ship::canMoveRight(Game * game_screen)
{
	return game_screen->canMoveX((GameObject *)this, DIRECTION_RIGHT);
}

bool Ship::MoveDown(Game * game_screen)
{
	//Lets go down!
	if (this->canMoveDown(game_screen))
	{
		EraseDrawing();
		vector<Object_location_t> & locations = get_locations();
		for (vector<Object_location_t>::iterator it = locations.begin(); it != locations.end(); ++it)
		{
			if (it->y == UPPER_Y)
			{
				it->y = it->y % UPPER_Y;
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

bool Ship::MoveUp(Game * game_screen)
{
	//Lets go up!
	if (this->canMoveUp(game_screen))
	{
		EraseDrawing();
		vector<Object_location_t> & locations = get_locations();
		for (vector<Object_location_t>::iterator it = locations.begin(); it != locations.end(); ++it)
		{
			if (it->y == 0)
			{
				it->y = UPPER_Y;
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

bool Ship::MoveLeft(Game * game_screen)
{
	//Lets go down!
	if (this->canMoveLeft(game_screen))
	{
		EraseDrawing();
		vector<Object_location_t> & locations = get_locations();
		for (vector<Object_location_t>::iterator it = locations.begin(); it != locations.end(); ++it)
		{
			if (it->x == 0)
			{
				it->x = UPPER_X;
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

bool Ship::MoveRight(Game * game_screen)
{
	//Lets go down!
	if (this->canMoveRight(game_screen))
	{
		EraseDrawing();
		vector<Object_location_t> & locations = get_locations();
		for (vector<Object_location_t>::iterator it = locations.begin(); it != locations.end(); ++it)
		{
			if (it->x == UPPER_X)
			{
				it->x = it->x % UPPER_X;
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

void Ship::draw(Canvas & canvas)
{
	vector<Object_location_t> locations = get_locations();
	for (vector<Object_location_t>::iterator it = locations.begin(); it != locations.end(); ++it)
	{
		canvas.setCharacter(it->x, it->y, this->texture);
	}
}

void Ship::EraseDrawing()
{
	vector<Object_location_t> & locations = get_locations();
	for (vector<Object_location_t>::iterator it = locations.begin(); it != locations.end(); ++it)
	{
		gotoxy(it->x, it->y);
		cout << " ";
	}
}
