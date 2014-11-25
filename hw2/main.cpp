

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

	Object_location_t location_arr_strange[8];
	location_arr_strange[0].x = 10;
	location_arr_strange[0].y = 5;
	location_arr_strange[1].x = 11;
	location_arr_strange[1].y = 5;
	location_arr_strange[2].x = 12;
	location_arr_strange[2].y = 5;
	location_arr_strange[3].x = 12;
	location_arr_strange[3].y = 6;
	location_arr_strange[4].x = 12;
	location_arr_strange[4].y = 7;
	location_arr_strange[5].x = 12;
	location_arr_strange[5].y = 8;
	location_arr_strange[6].x = 11;
	location_arr_strange[6].y = 8;
	location_arr_strange[7].x = 10;
	location_arr_strange[7].y = 8;

	Item * item_strange1 = new Item(7, location_arr_strange, 8);
	game->add_item(*item_strange1);

	Object_location_t location_arr_strange2[4];
	location_arr_strange2[0].x = 10;
	location_arr_strange2[0].y = 6;
	location_arr_strange2[1].x = 11;
	location_arr_strange2[1].y = 6;
	location_arr_strange2[2].x = 10;
	location_arr_strange2[2].y = 7;
	location_arr_strange2[3].x = 11;
	location_arr_strange2[3].y = 7;

	/*Item * item_strange2 = new Item(2, location_arr_strange2, 4);
	game->add_item(*item_strange2);*/

	Object_location_t location_arr[4];
	location_arr[0].x = 1;
	location_arr[0].y = 1;
	location_arr[1].x = 2;
	location_arr[1].y = 2;
	location_arr[2].x = 3;
	location_arr[2].y = 3;
	location_arr[3].x = 4;
	location_arr[3].y = 4;


	Object_location_t location_arr2[1];
	location_arr2[0].x = 4;
	location_arr2[0].y = 7;


	Item * item1 = new Item(4, location_arr, 4);
	game->add_item(*item1);



	Item * item2 = new Item(1, location_arr2, 1);
	game->add_item(*item2);

	Object_location_t location_arr3[1];
	location_arr3[0].x = 15;
	location_arr3[0].y = 20;

	Item * item3 = new Item(9, location_arr3, 1);
	game->add_item(*item3);

	Object_location_t location_arr4[1];
	location_arr4[0].x = 15;
	location_arr4[0].y = 5;

	Item * item4 = new Item(3, location_arr4, 1);
	game->add_item(*item4);

	Object_location_t location_arr5[1];
	location_arr5[0].x = 1;
	location_arr5[0].y = 20;

	Item * item5 = new Item(3, location_arr5, 1);
	game->add_item(*item5);

	ExitPoint_location_t exit_location;
	exit_location.x = 18;
	exit_location.y = 5;
	ExitPoint * exit = new ExitPoint(exit_location);
	game->set_exitpoint(exit);

	SmallShip smallship(12, 12);
	game->set_small_ship(smallship);

	BigShip bigship(18, 12);
	game->set_big_ship(bigship);

	Wall * walls[4 * 8 + 4 * 6 + 1];
	size_t walls_counter = 0;

	for (unsigned i = 0; i < 8; i++)
	{
		walls[walls_counter] = new Wall(i, 0);
		game->add_wall(*walls[walls_counter++]);
		walls[walls_counter] = new Wall(i, game->getCanvas().getHeight() - 1);
		game->add_wall(*walls[walls_counter++]);

		walls[walls_counter] = new Wall(game->getCanvas().getWidth() - 1 - i, 0);
		game->add_wall(*walls[walls_counter++]);
		walls[walls_counter] = new Wall(game->getCanvas().getWidth() - 1 - i, game->getCanvas().getHeight() - 1);
		game->add_wall(*walls[walls_counter++]);
	}

	for (unsigned i = 0; i < 6; i++)
	{
		walls[walls_counter] = new Wall(0, i + 1);
		game->add_wall(*walls[walls_counter++]);
		walls[walls_counter] = new Wall(game->getCanvas().getWidth() - 1, i + 1);
		game->add_wall(*walls[walls_counter++]);

		walls[walls_counter] = new Wall(0, game->getCanvas().getHeight() - 2 - i);
		game->add_wall(*walls[walls_counter++]);
		walls[walls_counter] = new Wall(game->getCanvas().getWidth() - 1, game->getCanvas().getHeight() - 2 - i);
		game->add_wall(*walls[walls_counter++]);
	}

	walls[walls_counter] = new Wall(1, 3);
	game->add_wall(*walls[walls_counter++]);

	game->Run();

	for (size_t i = 0; i < walls_counter; i++)
	{
		delete walls[i];
	}

	delete exit;
	delete item1;
	delete item2;
	delete item3;
	delete item4;
	delete item5;

	delete item_strange1;

	delete game;

}