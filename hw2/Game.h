#ifndef _GAME_H_
#define _GAME_H_
#include <vector>
#include "Item.h"
#include "BigShip.h"
#include "SmallShip.h"
#include "Wall.h"
#include "ExitPoint.h"
#include "Canvas.h"
using namespace std;

#define MAX_OF_EACH_OBSTECLE 10
#define MAX_OF_EACH_WALL_BLOCK 1000

// Forward dec
class Item;
class Ship;
class BigShip;
class SmallShip;
class Wall;
class ExitPoint;
class Canvas;

class Game
{
private:
	vector<Item> items_vec;
	/*Item * items[MAX_OF_EACH_OBSTECLE];
	size_t num_of_items;*/

	//vector<Ship> ships_vec;
	BigShip * big_ship;
	SmallShip * small_ship;
	/*Ship * ships[MAX_OF_EACH_OBSTECLE];
	size_t num_of_ships;*/

	vector<Wall> walls_vec;
	/*Wall * walls[MAX_OF_EACH_WALL_BLOCK];
	size_t num_of_walls*/

	ExitPoint * exit_point;

	Canvas canvas;

	void draw_all();
	//void erase_all();
public:
	void Run();

	void add_item(Item & item)
	{
		//items[num_of_items++] = item;
		items_vec.push_back(item);
	};

	void add_ship(Ship & ship)
	{
		//ships[num_of_ships++] = ship;
		ships_vec.push_back(ship);
	};

	void set_big_ship(BigShip & bigship)
	{
		big_ship = bigship;
	};

	void set_small_ship(SmallShip & smallship)
	{
		small_ship = smallship;
	};

	void add_wall(Wall & wall)
	{
		//walls[num_of_walls++] = wall;
		walls_vec.push_back(wall);
	};

	void set_exitpoint(ExitPoint * exit)
	{
		exit_point = exit;
	};

	size_t get_num_of_items()
	{
		return items_vec.size();
		//return num_of_items;
	};

	size_t get_num_of_ships()
	{
		return 2;
		//return num_of_ships;
	};

	size_t get_num_of_walls()
	{
		return walls_vec.size();
		//return num_of_walls;
	};

	Item * get_item(size_t index)
	{
		//return items_vec.
		return items[index];
	};

	Game() : exit_point(NULL), canvas(80, 24)
	{
		/*num_of_items = num_of_ships = num_of_walls = 0;*/
	};

	Canvas & getCanvas()
	{
		return (this->canvas);
	};
};

#endif