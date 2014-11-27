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

	bool MoveUp(Game * game_screen);
	bool MoveDown(Game * game_screen);
	bool MoveLeft(Game * game_screen);
	bool MoveRight(Game * game_screen);
public:
	Item(int item_number, const Object_location_t locations_array[], size_t locations_array_len);

	void draw(Canvas & canvas)
	{
		vector<Object_location_t> & locations = get_locations();
		for (vector<Object_location_t>::iterator it = locations.begin(); it != locations.end(); ++it)
		{
			canvas.setCharacter(it->x, it->y, item_number);
		}
	};

	void EraseDrawing(Canvas & canvas)
	{
		vector<Object_location_t> & locations = get_locations();
		for (vector<Object_location_t>::iterator it = locations.begin(); it != locations.end(); ++it)
		{
			canvas.setCharacter(it->x, it->y, ' ');
		}
	}

	bool canMoveDown(Game * screen);
	bool canMoveUp(Game * game_screen);
	bool canMoveLeft(Game * game_screen);
	bool canMoveRight(Game * game_screen);
	bool canItemMoveX(Game * game_screen, Direction direction);

	bool MoveX(Game * game_screen, Direction direction);
};
#endif