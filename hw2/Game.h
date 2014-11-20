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
	vector<Wall> walls_vec;

	BigShip * big_ship;
	SmallShip * small_ship;

	ExitPoint * exit_point;

	Canvas canvas;

	void draw_all();
	//void erase_all();
public:
	void Run();

	void add_item(Item & item)
	{
		items_vec.push_back(item);
	};


	void set_big_ship(BigShip & bigship)
	{
		big_ship = &bigship;
	};

	void set_small_ship(SmallShip & smallship)
	{
		small_ship = &smallship;
	};

	void add_wall(Wall & wall)
	{
		walls_vec.push_back(wall);
	};

	void set_exitpoint(ExitPoint * exit)
	{
		exit_point = exit;
	};

	size_t get_num_of_items()
	{
		return items_vec.size();
	};

	size_t get_num_of_ships()
	{
		return 2;
	};

	size_t get_num_of_walls()
	{
		return walls_vec.size();
	};


	vector<Item>::iterator items_iterator_begin()
	{
		return items_vec.begin();
	};

	vector<Item>::iterator items_iterator_end()
	{
		return items_vec.end();
	};

	Game() : exit_point(NULL), canvas(80, 24)
	{
	};

	Canvas & getCanvas()
	{
		return (this->canvas);
	};

	bool canMoveDown(Item * item);
};

#endif