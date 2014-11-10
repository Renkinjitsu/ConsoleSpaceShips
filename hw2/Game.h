#ifndef _GAME_H_
#define _GAME_H_

#include "Item.h"
#include "Ship.h"
#include "Wall.h"
#include "ExitPoint.h"


typedef class
{
public:
	class Item * items[10];
	size_t num_of_items;

	class Ship * ships[10];
	size_t num_of_ships;

	class Wall * walls[10];
	size_t num_of_walls;

	ExitPoint * exit_point;
}Game_t;
#endif