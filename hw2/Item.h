#ifndef _ITEM_H_
#define _ITEM_H_

#include "io_utils.h"
#include "Game.h"
using namespace std;

typedef struct
{
	int x;
	int y;
}Item_location_t;

class Item
{
private:
	int item_number;
	Item_location_t * locations_arr;
	size_t locations_arr_len;
	Item_location_t * location_with_y_most_lower = NULL;
	Item_location_t * location_with_y_most_higher = NULL;
	bool item_alive = false;


	void Draw()
	{
		for (size_t i = 0; i < locations_arr_len; i++)
		{
			gotoxy(locations_arr[i].x, locations_arr[i].y);
			cout << item_number;
		}
	};

	void EraseDrawing()
	{
		for (size_t i = 0; i < locations_arr_len; i++)
		{
			gotoxy(locations_arr[i].x, locations_arr[i].y);
			cout << " ";
		}
	}
public:
	Item(int item_number, const Item_location_t locations_array[], size_t locations_array_len )
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

		Draw();
		item_alive = true;

	};

	void advance_falling(Game_t * game_screen);
	bool standing_on(Item * items);
};
#endif