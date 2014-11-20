#ifndef _ITEM_H_
#define _ITEM_H_

#include "io_utils.h"
#include "Game.h"
#include "GameObject.h"
using namespace std;

class Wall;

class Item : public GameObject
{
private:
	int item_number;

	//vector<Item_location_t> locations;

public:
	Item(int item_number, const Item_location_t locations_array[], size_t locations_array_len);

	void draw(Canvas & canvas)
	{
		vector<Item_location_t> & locations = get_locations();
		for (vector<Item_location_t>::iterator it = locations.begin(); it != locations.end(); ++it)
		{
			canvas.setCharacter(it->x, it->y, item_number);
		}
	};

	void EraseDrawing()
	{
		vector<Item_location_t> & locations = get_locations();
		for (vector<Item_location_t>::iterator it = locations.begin(); it != locations.end(); ++it)
		{
			gotoxy(it->x, it->y);
			cout << " ";
		}
	}

	void advance_falling(class Game * game_screen);

	bool Item::is_standing_on_me(Item * item);
	bool Item::is_standing_on_me(Wall * wall);

	bool canMoveDown(Game * screen);
	void Item::MoveDown(Game * game_screen);
};
#endif