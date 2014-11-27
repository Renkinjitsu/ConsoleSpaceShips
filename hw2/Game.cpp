#include "io_utils.h"
#include "Game.h"

#define ESC 27

bool Game::checkBlockage(const Object_location_t & caller_location, const Object_location_t & possible_blocking_location, Direction block_type)
{
	switch (block_type)
	{
		case DIRECTION_UP:
		{
			if (caller_location.y == 0 && possible_blocking_location.y == this->canvas.getHeight() - 1)
			{

			}
			if (possible_blocking_location.x == caller_location.x)
			{
				if (caller_location.y == 0)
				{
					if (possible_blocking_location.y == this->canvas.getHeight() - 1)
					{
						return true;
					}
					
				}
				else if ((possible_blocking_location.y == caller_location.y - 1))
				{
					return true;
				}
			}
		}
		break;

		case DIRECTION_DOWN:
		{
			if (possible_blocking_location.x == caller_location.x && ((possible_blocking_location.y + this->canvas.getHeight() - 1) % this->canvas.getHeight()) == caller_location.y)
			{
				return true;
			}
		}
		break;

		case DIRECTION_LEFT:
		{
			if (possible_blocking_location.y == caller_location.y && possible_blocking_location.x == caller_location.x - 1)
			{
				return true;
			}
		}
		break;

		case DIRECTION_RIGHT:
		{
			if (possible_blocking_location.y == caller_location.y && possible_blocking_location.x == caller_location.x + 1)
			{
				return true;
			}
		}
		break;

	}

	return false;
}

bool Game::HasShipReachedExitPoint(Ship * ship)
{
	vector<Object_location_t> & ship_locations = ship->get_locations();
	for (vector<Object_location_t>::iterator it_ship_loc = ship_locations.begin(); it_ship_loc != ship_locations.end(); ++it_ship_loc)
	{
		Object_location_t & ship_location = *it_ship_loc;
		if (exit_point->IsAtExitPoint(ship_location))
		{
			return true;
		}
	}

	return false;
}

bool Game::ExplodeShipOnFreefalling(Item * item_falling, game_move_flags_t flags)
{
	return true;
}

//According to the specs , items one level above the ship should be carried with it. Ship carried only one item at a time.
bool Game::MoveItemsCarriedOnShip(vector<Object_location_t> & ship_locations, Direction direction, game_move_flags_t flags)
{
	for (vector<Object_location_t>::iterator it_ship_loc = ship_locations.begin(); it_ship_loc != ship_locations.end(); ++it_ship_loc)
	{
		//Iterate all items
		for (vector<Item>::iterator it_items = items_vec.begin(); it_items != items_vec.end(); ++it_items)
		{
			vector<Object_location_t> & item_locations = it_items->get_locations();
			for (vector<Object_location_t>::iterator it_item_loc = item_locations.begin(); it_item_loc != item_locations.end(); ++it_item_loc)
			{
				Object_location_t ItemLocation;
				ItemLocation.x = it_item_loc->x;
				ItemLocation.y = it_item_loc->y;

				Object_location_t ShipLocation;
				ShipLocation.x = it_ship_loc->x;
				ShipLocation.y = it_ship_loc->y;

				//Check if the ship carries the item from upwards
				if (checkBlockage(ShipLocation, ItemLocation, DIRECTION_UP))
				{

					if (it_items->MoveX(this, direction, flags))
					{
						return true;
					}

					return false;
				}
			}
		}
	}
	return true;
}

bool Game::canMoveX(GameObject * obj, Direction direction, game_move_flags_t flags)
{
	bool can_move = true;
	vector<Object_location_t> asking_object_locations = obj->get_locations();
	//check for wall blockage
	for (vector<Wall>::iterator it = walls_vec.begin(); it != walls_vec.end(); ++it)
	{
		Object_location_t wall_loc;
		wall_loc.x = it->getXstart();
		wall_loc.y = it->getYstart();

		for (vector<Object_location_t>::iterator it_obj_loc = asking_object_locations.begin(); it_obj_loc != asking_object_locations.end(); ++it_obj_loc)
		{
			Object_location_t & obj_loc = *it_obj_loc;
			if (checkBlockage(obj_loc, wall_loc, direction))
			{
				can_move = false;
				return can_move;
			}
		}
	}

	//check for item blockage
	for (vector<Item>::iterator it = items_vec.begin(); it != items_vec.end(); ++it)
	{
		Item & cur_item = *it;
		if (&cur_item != obj)
		{
			vector<Object_location_t> other_item_locations = it->get_locations();

			for (vector<Object_location_t>::iterator my_obj_loc_it = asking_object_locations.begin(); my_obj_loc_it != asking_object_locations.end(); ++my_obj_loc_it)
			{
				for (vector<Object_location_t>::iterator other_item_loc_it = other_item_locations.begin(); other_item_loc_it != other_item_locations.end(); ++other_item_loc_it)
				{
					Object_location_t MyItemLocation;
					MyItemLocation.x = my_obj_loc_it->x;
					MyItemLocation.y = my_obj_loc_it->y;

					Object_location_t OtherItemLocation;
					OtherItemLocation.x = other_item_loc_it->x;
					OtherItemLocation.y = other_item_loc_it->y;

					if (checkBlockage(MyItemLocation, OtherItemLocation, direction))
					{
						if (!it->MoveX(this, direction, flags))
						{
							can_move = false;
							return can_move;
						}
					}
				}
			}
		}
	}

	//check for ship blockage
	vector<Object_location_t> smallship_loc = small_ship->get_locations();
	vector<Object_location_t> bigship_loc = big_ship->get_locations();

	for (vector<Object_location_t>::iterator it_obj_loc = asking_object_locations.begin(); it_obj_loc != asking_object_locations.end(); ++it_obj_loc)
	{
		Object_location_t MyItemLocation;
		MyItemLocation.x = it_obj_loc->x;
		MyItemLocation.y = it_obj_loc->y;

		//check blockage with smallship
		for (vector<Object_location_t>::iterator it_smallship_loc = smallship_loc.begin(); it_smallship_loc != smallship_loc.end(); ++it_smallship_loc)
		{
			if (small_ship != obj)
			{
				Object_location_t SmallShipLocation;
				SmallShipLocation.x = it_smallship_loc->x;
				SmallShipLocation.y = it_smallship_loc->y;

				if (checkBlockage(MyItemLocation, SmallShipLocation, direction))
				{
					can_move = false;
					return can_move;
				}
			}
		}
		//check blockage with bigsship
		for (vector<Object_location_t>::iterator it_bigship_loc = bigship_loc.begin(); it_bigship_loc != bigship_loc.end(); ++it_bigship_loc)
		{
			if (big_ship != obj)
			{
				Object_location_t BigShipLocation;
				BigShipLocation.x = it_bigship_loc->x;
				BigShipLocation.y = it_bigship_loc->y;

				if (checkBlockage(MyItemLocation, BigShipLocation, direction))
				{
					can_move = false;
					return can_move;
				}
			}
		}

	}

	//check blockage with exit point (not relevent to ships - ships should go thru exit points)
	if (obj->get_type() != OBJECT_SHIP)
	{
		for (vector<Object_location_t>::iterator it_obj_loc = asking_object_locations.begin(); it_obj_loc != asking_object_locations.end(); ++it_obj_loc)
		{
			Object_location_t & cur_location = *it_obj_loc;
			Object_location_t & exit_point_location = exit_point->get_location();
			if (this->checkBlockage(cur_location, exit_point_location, direction))
			{
				can_move = false;
				return can_move;
			}
		}
	}

	return can_move;
}

void Game::draw_all()
{
	for (vector<Item>::iterator it = items_vec.begin(); it != items_vec.end(); ++it) {
		it->draw(this->canvas);
	}

	small_ship->draw(this->canvas);
	big_ship->draw(this->canvas);

	for (vector<Wall>::iterator it = walls_vec.begin(); it != walls_vec.end(); ++it) {
		it->draw(this->canvas);
	}

	if (exit_point != NULL)
	{
		exit_point->draw(this->canvas);
	}
};

void Game::erase_all()
{
	for (vector<Item>::iterator it = items_vec.begin(); it != items_vec.end(); ++it) {
		it->EraseDrawing(this->canvas);
	}

	small_ship->EraseDrawing(this->canvas);
	big_ship->EraseDrawing(this->canvas);

	for (vector<Wall>::iterator it = walls_vec.begin(); it != walls_vec.end(); ++it) {
		it->EraseDrawing(this->canvas);
	}

	if (exit_point != NULL)
	{
		exit_point->EraseDrawing(this->canvas);
	}
}

void Game::readUserInput()
{
	this->bigShipDir = DIRECTION_NONE;
	this->smallShipDir = DIRECTION_NONE;

	while (_kbhit() && this->isGameOver == false)
	{
		switch (_getch())
		{
			case ESC:
			{
				this->isGameOver = true;
		}
			break;

			case 'x':
			{
				this->smallShipDir = DIRECTION_DOWN;
			}
			break;

			case 'w':
			{
				this->smallShipDir = DIRECTION_UP;
			}
			break;

			case 'a':
			{
				this->smallShipDir = DIRECTION_LEFT;
			}
			break;

			case 'd':
			{
				this->smallShipDir = DIRECTION_RIGHT;
			}
			break;

			case 'i':
			{
				this->bigShipDir = DIRECTION_UP;
			}
			break;

			case 'm':
			{
				this->bigShipDir = DIRECTION_DOWN;
			}
			break;

			case 'j':
			{
				this->bigShipDir = DIRECTION_LEFT;
			}
			break;

			case 'l':
			{
				this->bigShipDir = DIRECTION_RIGHT;
			}
			break;
		} //End of 'switch'
	} //End of loop
}

void Game::moveShip(Ship & ship, Direction direction, game_move_flags_t flags)
{
	switch (direction)
	{
		case DIRECTION_DOWN:
		{
			ship.MoveDown(*this, flags);
			}
			break;

		case DIRECTION_UP:
			{
			ship.MoveUp(this, flags);
			}
			break;

		case DIRECTION_LEFT:
		{
			ship.MoveLeft(this, flags);
		}
		break;

		case DIRECTION_RIGHT:
		{
			ship.MoveRight(this, flags);
		}
		break;
	}
}

void Game::Run()
{
	this->draw_all();

	game_move_flags_t flags;
	flags.is_carried_on_ship = false;

	do
	{
		this->readUserInput();

		if (this->isGameOver)
		{
			break;
		}

		this->moveShip(*this->small_ship, this->smallShipDir, flags);
		this->moveShip(*this->big_ship, this->bigShipDir, flags);

		for(vector<Item>::iterator it = items_vec.begin(); it != items_vec.end(); ++it)
		{
			if (it->canMoveDown(this, flags))
			{
				it->MoveX(this, DIRECTION_DOWN, flags);
			}
		}

		if (small_ship->IsShipAlive() == false && big_ship->IsShipAlive() == false)
		{
			this->isGameOver = true;
		}

		//for each ship do IsAtExitPoint and if so - quit the loop

		Sleep(100);
	} while (isGameOver == false);


	this->erase_all();

	gotoxy(Canvas::getWidth() / 2, Canvas::getHeight() / 2);
	cout << "GAME OVER!";
}