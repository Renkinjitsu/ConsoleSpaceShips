#include "Item.h"
#include "config.h"

Item::Item(int item_number, const Item_location_t locations_array[], size_t locations_array_len)
{
	this->item_number = item_number;

	location_with_y_most_higher.x = 0;
	location_with_y_most_higher.y = 0;

	location_with_y_most_lower.x = UPPER_X;
	location_with_y_most_lower.y = UPPER_Y;

	/*locations_arr = new Item_location_t[locations_array_len];
	locations_arr_len = locations_array_len;*/



	for (size_t i = 0; i < locations_array_len; i++)
	{
		locations.push_back(locations_array[i]);
		//locations_arr[i] = locations_array[i];
		/*if (location_with_y_most_lower == NULL || locations_array[i].y < location_with_y_most_lower->y)
		{
			location_with_y_most_lower = &locations_array[i];
		}

		if (location_with_y_most_higher == NULL || locations_arr[i].y > location_with_y_most_higher->y)
		{
			location_with_y_most_higher = &locations_arr[i];
		}*/
	}

	for (vector<Item_location_t>::iterator it = locations.begin(); it != locations.end(); ++it)
	{
		Item_location_t & item_loc = *it;
		if (/*location_with_y_most_lower == NULL ||*/ it->y < location_with_y_most_lower.y)
		{
			location_with_y_most_lower = item_loc;
		}

		if (/*location_with_y_most_higher == NULL ||*/ it->y > location_with_y_most_higher.y)
		{
			location_with_y_most_higher = item_loc;
		}
	}

	//Draw();
	//item_alive = true;

}

void Item::advance_falling(Game * game_screen)
{
	for (vector<Item>::iterator it = game_screen->items_iterator_begin(); it != game_screen->items_iterator_end(); ++it) {
		Item & item = *it;
		if (this->is_standing_on_me(&item))
		{
			return;
		}
	}




	//Lets go down!
	EraseDrawing();

	for (vector<Item_location_t>::iterator it = locations.begin(); it != locations.end(); ++it)
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
}

bool Item::is_standing_on_me(Item * item)
{
	if (item == this)
	{ // He is me
		return false;
	}

	for (vector<Item_location_t>::iterator it = this->locations.begin(); it != this->locations.end(); ++it)
	{
		for (vector<Item_location_t>::iterator it_other = item->locations.begin(); it_other != item->locations.end(); ++it_other)
		{
			if (it_other->y + 1 == it->y && it_other->x + 1 == it->x)
			{
				return true;
			}
		}
	}

	/*for (size_t j = 0; j < this->locations_arr_len; j++)
	{
		for (size_t i = 0; i < item->locations_arr_len; i++)
		{
			if (item->locations_arr[i].y + 1 == this->locations_arr[j].y && item->locations_arr[i].x + 1 == this->locations_arr[j].x)
			{
				return true;
			}
		}
	}*/

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