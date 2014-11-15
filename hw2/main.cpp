

#include "io_utils.h"
#include "Item.h"
#include "Game.h"

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

	ExitPoint_location_t exit_location;
	exit_location.x = 10;
	exit_location.y = 5;
	ExitPoint * exit = new ExitPoint(exit_location);
	game->set_exitpoint(exit);

	game->Run();


}