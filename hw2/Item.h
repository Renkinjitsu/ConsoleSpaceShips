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

	void EraseDrawing()
	{
		vector<Object_location_t> & locations = get_locations();
		for (vector<Object_location_t>::iterator it = locations.begin(); it != locations.end(); ++it)
		{
			gotoxy(it->x, it->y);
			cout << " ";
		}
	}

	bool canMoveDown(Game * screen, game_move_flags_t flags);
	bool canMoveUp(Game * game_screen, game_move_flags_t flags);
	bool canMoveLeft(Game * game_screen, game_move_flags_t flags);
	bool canMoveRight(Game * game_screen, game_move_flags_t flags);
	bool canItemMoveX(Game * game_screen, game_direction_e direction, game_move_flags_t flags);
	bool MoveX(Game * game_screen, game_direction_e direction, game_move_flags_t flags);
	bool MoveUp(Game * game_screen, game_move_flags_t flags);
	bool MoveDown(Game * game_screen, game_move_flags_t flags);
	bool MoveLeft(Game * game_screen, game_move_flags_t flags);
	bool MoveRight(Game * game_screen, game_move_flags_t flags);
};
#endif