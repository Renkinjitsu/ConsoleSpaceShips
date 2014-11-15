#ifndef _GAME_H_
#define _GAME_H_

#include "Item.h"
#include "Ship.h"
#include "Wall.h"
#include "ExitPoint.h"
using namespace std;

#define MAX_OF_EACH_OBSTECLE 10

// Forward dec
class Item;
class Ship;
class Wall;

class Game
{
private:
	Item * items[MAX_OF_EACH_OBSTECLE];
	size_t num_of_items;

	Ship * ships[MAX_OF_EACH_OBSTECLE];
	size_t num_of_ships;

	Wall * walls[MAX_OF_EACH_OBSTECLE];
	size_t num_of_walls;

	ExitPoint * exit_point;

	void draw_all();
public:
	void Run();

	void add_item(Item * item)
	{
		items[num_of_items++] = item;
	};

	void add_ship(Ship * ship)
	{
		ships[num_of_ships++] = ship;
	};

	void add_wall(Wall * wall)
	{
		walls[num_of_walls++] = wall;
	};

	void set_exitpoint(ExitPoint * exit)
	{
		exit_point = exit;
	};

	size_t get_num_of_items()
	{
		return num_of_items;
	};

	size_t get_num_of_ships()
	{
		return num_of_ships;
	};

	size_t get_num_of_walls()
	{
		return num_of_walls;
	};

	Item * get_item(size_t index)
	{
		return items[index];
	};

	Game(): exit_point(NULL)
	{
		num_of_items = num_of_ships = num_of_walls = 0;
	};
};

#endif