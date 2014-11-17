#ifndef _ITEM_H_
#define _ITEM_H_

#include "io_utils.h"
#include "Game.h"
using namespace std;

class Wall;

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
public:
	Item(int item_number, const Item_location_t locations_array[], size_t locations_array_len);

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

	void advance_falling(class Game * game_screen);

	bool Item::is_standing_on_me(Item * item);
	bool Item::is_standing_on_me(Wall * wall);
};
#endif