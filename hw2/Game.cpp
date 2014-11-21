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

bool Game::canMoveX(Item * item, game_direction_e direction)
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
		exit_point->Draw();
	}
};

/*void Game::erase_all()
{
	size_t i;

	for (i = 0; i < num_of_items; i++)
	{
		items[i]->EraseDrawing();
	}

	for (i = 0; i < num_of_ships; i++)
	{
		ships[i]->EraseDrawing();
	}

	for (i = 0; i < num_of_walls; i++)
	{
		walls[i]->EraseDrawing();
	}

	if (exit_point != NULL)
	{
		exit_point->EraseDrawing();
	}
}*/

void Game::Run()
{
	this->draw_all();
	// esc (Ascii 27) ends the loop
	while (!_kbhit() || _getch() != ESC)
	{
		for (vector<Item>::iterator it = items_vec.begin(); it != items_vec.end(); ++it) {
			it->MoveDown(this);
		}

		//for each ship do IsAtExitPoint and if so - quit the loop

		Sleep(150);
	}


	//this->erase_all();

	gotoxy(UPPER_X / 2, UPPER_Y / 2);
	cout << "GAME OVER!";
}