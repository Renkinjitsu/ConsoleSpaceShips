#include "Item.h"
#include "config.h"

void Item::advance_falling(Game * game_screen)
{
	if (item_alive)
	{

		for (size_t i = 0; i < game_screen->get_num_of_items(); i++)
		{
			if (this->standing_on(game_screen->get_item(i)))
			{
				return;
			}
		}


		EraseDrawing();
		if (this->location_with_y_most_higher->y <= UPPER_Y)
		{ // lower the entire structure
			for (size_t i = 0; i < locations_arr_len; i++)
			{
				locations_arr[i].y++;
			}

			Draw();
		}
		else
		{
			/*for (size_t i = 0; i < locations_arr_len; i++)
			{
			locations_arr[i].y = 1 + (UPPER_Y - locations_arr[i].y);
			}*/

			//Item disappeared and died
			item_alive = false;
		}
	}
}

bool Item::standing_on(Item * item)
{
	if (item == this)
	{
		return false;
	}

	for (size_t j = 0; j < this->locations_arr_len; j++)
	{
		for (size_t i = 0; i < item->locations_arr_len; i++)
		{
			if (item->locations_arr[i].y == this->locations_arr[j].y + 1)
			{
				return true;
			}
		}
	}

	return false;
}