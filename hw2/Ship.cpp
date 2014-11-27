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

bool Ship::canMoveDown(Game * game_screen, game_move_flags_t flags)
{
	return game_screen->canMoveX((GameObject *)this, DIRECTION_DOWN, flags);
}

bool Ship::canMoveUp(Game * game_screen, game_move_flags_t flags)
{
	return game_screen->canMoveX((GameObject *)this, DIRECTION_UP, flags);
}

bool Ship::canMoveLeft(Game * game_screen, game_move_flags_t flags)
{
	return game_screen->canMoveX((GameObject *)this, DIRECTION_LEFT, flags);
}

bool Ship::canMoveRight(Game * game_screen, game_move_flags_t flags)
{
	return game_screen->canMoveX((GameObject *)this, DIRECTION_RIGHT, flags);
}

bool Ship::MoveDown(Game & game_screen, game_move_flags_t flags)
{
	//Lets go down!
	if (this->IsShipAlive() && this->canMoveDown(&game_screen, flags))
	{
		vector<Object_location_t> & locations = get_locations();
		vector<Object_location_t> prev_locations = locations;
		EraseDrawing(game_screen.getCanvas());
		GameObject::moveDown();

		/* MoveItemsCarriedOnShip checks the "carried items" location via the previous locations of the ship (cuz it just went down)
			Its done on MoveDown action in this way becasue of the issue of this ship as the "blocking" object for the item above.
		*/

		// explosion_allowed is false because they said an item falling after the ship went down doesnt mean "free falling" => the ship shouldnt explode
		flags.explosion_allowed = false;
		game_screen.MoveItemsCarriedOnShip(prev_locations, DIRECTION_DOWN, flags);

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
	if (this->IsShipAlive() && this->canMoveUp(game_screen, flags))
	{
		vector<Object_location_t> & locations = get_locations();
		game_screen->MoveItemsCarriedOnShip(locations, DIRECTION_UP, flags);
		EraseDrawing(game_screen->getCanvas());
		GameObject::moveUp();

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
	if (this->IsShipAlive() && this->canMoveLeft(game_screen, flags))
	{
		vector<Object_location_t> & locations = get_locations();
		game_screen->MoveItemsCarriedOnShip(locations, DIRECTION_LEFT, flags);
		EraseDrawing(game_screen->getCanvas());
		GameObject::moveLeft();

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
	if (this->IsShipAlive() && this->canMoveRight(game_screen, flags))
	{
		vector<Object_location_t> & locations = get_locations();
		game_screen->MoveItemsCarriedOnShip(locations, DIRECTION_RIGHT, flags);

		EraseDrawing(game_screen->getCanvas());
		GameObject::moveRight();

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
