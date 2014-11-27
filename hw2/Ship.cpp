#include "Definitions.h"
#include "Ship.h"
#include "Game.h"

Ship::Ship(unsigned xPositin, unsigned yPosition,
	unsigned width, unsigned height,
	char texture) : GameObject(OBJECT_SHIP, xPositin, yPosition), texture(texture)
{
	this->is_alive = true;

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

bool Ship::IsShipAlive()
{
	return this->is_alive;
}

void Ship::SetShipAlive(bool is_alive)
{
	this->is_alive = is_alive;
	if (!this->is_alive)
	{
		this->KillShip();
	}
}

void Ship::KillShip()
{
	this->clear_locations();
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

bool Ship::MoveDown(Game & game_screen, game_move_flags_t flags)
{
	//Lets go down!
	if (this->IsShipAlive() && this->canMoveDown(&game_screen))
	{
		EraseDrawing(game_screen.getCanvas());
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

		if (game_screen.HasShipReachedExitPoint(this))
		{
			this->SetShipAlive(false);
		}
		else
		{
			draw(game_screen.getCanvas());
		}

		return true;
	}
	return false;
}

bool Ship::MoveUp(Game * game_screen, game_move_flags_t flags)
{
	//Lets go up!
	if (this->IsShipAlive() && this->canMoveUp(game_screen))
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

		if (game_screen->HasShipReachedExitPoint(this))
		{
			this->SetShipAlive(false);
		}
		else draw(game_screen->getCanvas());

		return true;
	}
	return false;
}

bool Ship::MoveLeft(Game * game_screen, game_move_flags_t flags)
{
	//Lets go down!
	if (this->IsShipAlive() && this->canMoveLeft(game_screen))
	{

		game_screen->MoveItemsCarriedOnShip(this, DIRECTION_LEFT, flags);
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

		if (game_screen->HasShipReachedExitPoint(this))
		{
			this->SetShipAlive(false);
		}
		else draw(game_screen->getCanvas());

		return true;
	}
	return false;
}

bool Ship::MoveRight(Game * game_screen, game_move_flags_t flags)
{
	//Lets go down!
	if (this->IsShipAlive() && this->canMoveRight(game_screen))
	{
		game_screen->MoveItemsCarriedOnShip(this, DIRECTION_RIGHT, flags);

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

		if (game_screen->HasShipReachedExitPoint(this))
		{
			this->SetShipAlive(false);
		}
		else draw(game_screen->getCanvas());

	

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

void Ship::EraseDrawing(Canvas & canvas)
{
	vector<Object_location_t> & locations = get_locations();
	for (vector<Object_location_t>::iterator it = locations.begin(); it != locations.end(); ++it)
	{
		canvas.setCharacter(it->x, it->y, ' ');
	}
}
