#include "Item.h"
#include "config.h"

Item::Item(int item_number, const Item_location_t locations_array[], size_t locations_array_len)
{
	this->item_number = item_number;

	locations_arr = new Item_location_t[locations_array_len];
	locations_arr_len = locations_array_len;

	for (size_t i = 0; i < locations_array_len; i++)
	{
		locations_arr[i] = locations_array[i];
		if (location_with_y_most_lower == NULL || locations_arr[i].y < location_with_y_most_lower->y)
		{
			location_with_y_most_lower = &locations_arr[i];
		}

		if (location_with_y_most_higher == NULL || locations_arr[i].y > location_with_y_most_higher->y)
		{
			location_with_y_most_higher = &locations_arr[i];
		}
	}

	//Draw();
	item_alive = true;

}

void Item::advance_falling(Game * game_screen)
{
	if (item_alive)
	{
		for (size_t i = 0; i < game_screen->get_num_of_items(); i++)
		{
			if (this->is_standing_on_me(game_screen->get_item(i)))
			{
				return;
			}
		}


		EraseDrawing();
		if (this->location_with_y_most_higher->y < UPPER_Y)
		{ // lower the entire structure
			for (size_t i = 0; i < locations_arr_len; i++)
			{
				locations_arr[i].y++;
			}

			draw(game_screen->getCanvas());
		}
		else
		{

			//Item disappeared and died
			item_alive = false;
		}
	}
}

bool Item::is_standing_on_me(Item * item)
{
	if (item == this)
	{ // He is me
		return false;
	}

	for (size_t j = 0; j < this->locations_arr_len; j++)
	{
		for (size_t i = 0; i < item->locations_arr_len; i++)
		{
			if (item->locations_arr[i].y + 1 == this->locations_arr[j].y && item->locations_arr[i].x + 1 == this->locations_arr[j].x)
			{
				return true;
			}
		}
	}

	return false;
}

/*bool Item::standing_on(Wall * wall)
{
	for (size_t j = 0; j < this->locations_arr_len; j++)
	{
		for (size_t i = 0; i < wall->locations_arr_len; i++)
		{
			if (item->locations_arr[i].y == this->locations_arr[j].y + 1 && item->locations_arr[i].x == this->locations_arr[j].x + 1)
			{
				return true;
			}
		}
	}

	return false;
}*/