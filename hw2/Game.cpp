#include "io_utils.h"
#include "Game.h"

#define ESC 27

bool Game::checkBlockage(const Object_location_t & caller_location, const Object_location_t & possible_blocking_location, game_direction_e block_type)
{
	switch (block_type)
	{
		case DIRECTION_UP:
		{
			if (possible_blocking_location.x == caller_location.x && possible_blocking_location.y == caller_location.y - 1)
			{
				return true;
			}
		}
		break;

		case DIRECTION_DOWN:
		{
			if (possible_blocking_location.x == caller_location.x && possible_blocking_location.y == caller_location.y + 1)
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

//According to the specs , items one level above the ship should be carried with it. Ship carried only one item at a time.
bool Game::MoveItemsCarriedOnShip(Ship * ship, game_direction_e direction, game_move_flags_t flags)
{
	vector<Object_location_t> & ship_locations = ship->get_locations();
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

bool Game::canMoveX(GameObject * obj, game_direction_e direction, game_move_flags_t flags)
{
	bool can_move = true;
	//check for wall blockage
	for (vector<Wall>::iterator it = walls_vec.begin(); it != walls_vec.end(); ++it)
	{
		Object_location_t wall_loc;
		wall_loc.x = it->getXstart();
		wall_loc.y = it->getYstart();

		vector<Object_location_t> obj_locations = obj->get_locations();

		for (vector<Object_location_t>::iterator it_obj_loc = obj_locations.begin(); it_obj_loc != obj_locations.end(); ++it_obj_loc)
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
			vector<Object_location_t> my_obj_locations = obj->get_locations();
			vector<Object_location_t> other_item_locations = it->get_locations();

			for (vector<Object_location_t>::iterator my_obj_loc_it = my_obj_locations.begin(); my_obj_loc_it != my_obj_locations.end(); ++my_obj_loc_it)
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
							//return true;
							can_move = false;
						}

						//can_move = false;

						//return false;
					}
				}
			}
		}
	}

	//check for ship blockage
	vector<Object_location_t> smallship_loc = small_ship->get_locations();
	vector<Object_location_t> bigship_loc = big_ship->get_locations();

	vector<Object_location_t> obj_locations = obj->get_locations();
	for (vector<Object_location_t>::iterator it_obj_loc = obj_locations.begin(); it_obj_loc != obj_locations.end(); ++it_obj_loc)
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

	return can_move;
}

/*bool Game::canMoveX(Item * item, enum game_direction_e direction)
{
	//check for wall blockage
	for (vector<Wall>::iterator it = walls_vec.begin(); it != walls_vec.end(); ++it)
	{
		Object_location_t wall_loc;
		wall_loc.x = it->getXstart();
		wall_loc.y = it->getYstart();

		vector<Object_location_t> item_locations = item->get_locations();

		for (vector<Object_location_t>::iterator it_item_loc = item_locations.begin(); it_item_loc != item_locations.end(); ++it_item_loc)
		{
			Object_location_t & item_loc = *it_item_loc;
			if (checkBlockage(item_loc, wall_loc, direction))
			{
				return false;
			}
		}
	}

	//check for item blockage
	for (vector<Item>::iterator it = items_vec.begin(); it != items_vec.end(); ++it)
	{
		Item & cur_item = *it;
		if (&cur_item != item)
		{
			vector<Object_location_t> my_item_locations = item->get_locations();
			vector<Object_location_t> other_item_locations = it->get_locations();

			for (vector<Object_location_t>::iterator my_item_loc_it = my_item_locations.begin(); my_item_loc_it != my_item_locations.end(); ++my_item_loc_it)
			{
				for (vector<Object_location_t>::iterator other_item_loc_it = other_item_locations.begin(); other_item_loc_it != other_item_locations.end(); ++other_item_loc_it)
				{
					Object_location_t MyItemLocation;
					MyItemLocation.x = my_item_loc_it->x;
					MyItemLocation.y = my_item_loc_it->y;

					Object_location_t OtherItemLocation;
					OtherItemLocation.x = other_item_loc_it->x;
					OtherItemLocation.y = other_item_loc_it->y;

					if (checkBlockage(MyItemLocation, OtherItemLocation, direction))
					{
						if (it->MoveX(this, direction))
						{

						}
						return false;
					}
				}
			}
		}
	}

	//check for ship blockage
	vector<Object_location_t> item_locations = item->get_locations();
	for (vector<Object_location_t>::iterator it_item_loc = item_locations.begin(); it_item_loc != item_locations.end(); ++it_item_loc)
	{

	}

	return true;
}*/

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
		exit_point->Draw();
	}
};

void Game::erase_all()
{
	size_t i;

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

void Game::Run()
{
	this->draw_all();
	char key = ' ';
	game_move_flags_t flags;
	flags.is_carried_on_ship = false;
	// esc (Ascii 27) ends the loop
	while (!_kbhit() || (key = _getch()) != ESC)
	{

		for (vector<Item>::iterator it = items_vec.begin(); it != items_vec.end(); ++it) {
			it->MoveDown(this, flags);
		}

		switch (key)
		{
			case 'x':
			{
				small_ship->MoveDown(this, flags);
				key = ' ';
			}
			break;

			case 'w':
			{
				small_ship->MoveUp(this, flags);
				key = ' ';
			}
			break;

			case 'd':
			{
				small_ship->MoveRight(this, flags);
				key = ' ';
			}
			break;

			case 'a':
			{
				small_ship->MoveLeft(this, flags);
				key = ' ';
			}
			break;

			case 'i':
			{
				big_ship->MoveUp(this, flags);
				key = ' ';
			}
			break;

			case 'j':
			{
				big_ship->MoveLeft(this, flags);
				key = ' ';
			}
			break;

			case 'l':
			{
				big_ship->MoveRight(this, flags);
				key = ' ';
			}
			break;

			case 'm':
			{
				big_ship->MoveDown(this, flags);
				key = ' ';
			}
			break;
		}


		//for each ship do IsAtExitPoint and if so - quit the loop

		Sleep(150);
	}


	this->erase_all();

	gotoxy(UPPER_X / 2, UPPER_Y / 2);
	cout << "GAME OVER!";
}