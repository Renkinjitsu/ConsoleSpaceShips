

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


	Game_t * game_screen = new Game_t();

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


	game_screen->items[0] = new Item(4, location_arr, 4);

	game_screen->items[1] = new Item(6, location_arr2, 2);

	game_screen->num_of_items = 2;

	// esc (Ascii 27) ends the loop
	while (!_kbhit() || _getch() != ESC)
	{
		game_screen->items[0]->advance_falling(game_screen);
		Sleep(50);
		/*gotoxy(x, y);
		cout << "*" << endl;
		Sleep(50);
		gotoxy(x, y);
		cout << " " << endl;
		++x;
		if (x>79) { x = 1; }
		++y;
		if (y>20) { y = 1; }*/
	}
	return 0;
}