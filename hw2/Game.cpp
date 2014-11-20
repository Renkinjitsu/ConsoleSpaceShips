#include "io_utils.h"
#include "Game.h"

#define ESC 27

bool Game::canMoveDown(Item * item)
{
	//check for wall blockage
	for (vector<Wall>::iterator it = walls_vec.begin(); it != walls_vec.end(); ++it)
	{
		unsigned wallX = it->getXstart();
		unsigned wallY = it->getYstart();

		vector<Item_location_t> item_locations = item->get_locations();

		for (vector<Item_location_t>::iterator it_item_loc = item_locations.begin(); it_item_loc != item_locations.end(); ++it_item_loc)
		{
			if (wallX == it_item_loc->x && wallY == it_item_loc->y + 1)
			{
				return false;
			}
		}
	}

	for (vector<Item>::iterator it = items_vec.begin(); it != items_vec.end(); ++it) 
	{
		vector<Item_location_t> my_item_locations = item->get_locations();
		vector<Item_location_t> other_item_locations = it->get_locations();

		for (vector<Item_location_t>::iterator my_item_loc_it = my_item_locations.begin(); my_item_loc_it != my_item_locations.end(); ++my_item_loc_it)
		{
			for (vector<Item_location_t>::iterator other_item_loc_it = other_item_locations.begin(); other_item_loc_it != other_item_locations.end(); ++other_item_loc_it)
			{
				unsigned MyItemPointX = my_item_loc_it->x;
				unsigned MyItemPointY = my_item_loc_it->y;
				unsigned OtherItemPointX = other_item_loc_it->x;
				unsigned OtherItemPointY = other_item_loc_it->y;
				if (MyItemPointX == OtherItemPointX && OtherItemPointY == MyItemPointY + 1)
				{
					return it->canMoveDown(this);
				}
			}
		}
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