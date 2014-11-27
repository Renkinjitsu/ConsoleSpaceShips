#ifndef _GAME_H_
#define _GAME_H_
#include <vector>
#include "Definitions.h"
#include "Item.h"
#include "BigShip.h"
#include "SmallShip.h"
#include "Wall.h"
#include "ExitPoint.h"
#include "Canvas.h"
using namespace std;

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

	Direction bigShipDir;
	Direction smallShipDir;

	Canvas canvas;

	bool isGameOver;

	void draw_all();

	void readUserInput();
	void moveShip(Ship & ship, Direction direction, game_move_flags_t flags);

public:
	void Run();

	void erase_all();

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
		this->isGameOver = false;
	};

	Canvas & getCanvas()
	{
		return (this->canvas);
	};

	bool HasShipReachedExitPoint(Ship * ship);

	bool MoveItemsCarriedOnShip(Ship * ship, Direction direction, game_move_flags_t flags);
	bool canMoveDown(Item * item);
	bool canMoveUp(Item * item);
	//bool canMoveX(Item * item, enum game_direction_e direction);
	bool canMoveX(GameObject * obj, Direction direction);

	bool checkBlockage(const Object_location_t & caller_location, const Object_location_t & possible_blocking_location, Direction block_type);
};

#endif