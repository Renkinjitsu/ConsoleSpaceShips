

#include "io_utils.h"
#include "Item.h"
#include "Game.h"
#include "SmallShip.h"
#include "BigShip.h"
#include "Wall.h"
#include "Ship.h"

using namespace std;

#define ESC 27

int main()
{
	system("cls");

	int x = 1;
	int y = 1;

	Game * game = new Game();

	Item_location_t location_arr[4];
	location_arr[0].x = 1;
	location_arr[0].y = 1;
	location_arr[1].x = 2;
	location_arr[1].y = 2;
	location_arr[2].x = 3;
	location_arr[2].y = 3;
	location_arr[3].x = 4;
	location_arr[3].y = 4;


	Item_location_t location_arr2[1];
	location_arr2[0].x = 4;
	location_arr2[0].y = 7;


	game->add_item(new Item(4, location_arr, 4));

	game->add_item(new Item(6, location_arr2, 2));

	game->add_ship(new SmallShip(12, 12));
	game->add_ship(new BigShip(18, 12));

	for (unsigned i = 0; i < 8; i++)
	{
		game->add_wall(new Wall(i, 0));
		game->add_wall(new Wall(i, game->getCanvas()->getHeight() - 1));

		game->add_wall(new Wall(game->getCanvas()->getWidth() - 1 - i, 0));
		game->add_wall(new Wall(game->getCanvas()->getWidth() - 1 - i, game->getCanvas()->getHeight() - 1));
	}

	for (unsigned i = 0; i < 6; i++)
	{
		game->add_wall(new Wall(0, i + 1));
		game->add_wall(new Wall(game->getCanvas()->getWidth() - 1, i + 1));

		game->add_wall(new Wall(0, game->getCanvas()->getHeight() - 2 - i));
		game->add_wall(new Wall(game->getCanvas()->getWidth() - 1, game->getCanvas()->getHeight() - 2 - i));
	}

	game->Run();
}