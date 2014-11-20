#include "io_utils.h"
#include "Game.h"

#define ESC 27

void Game::draw_all()
{
	for (vector<Item>::iterator it = items_vec.begin(); it != items_vec.end(); ++it) {
		it->draw(this->canvas);
	}

	small_ship->draw(this->canvas);
	big_ship->draw(this->canvas);

	for (vector<Wall>::iterator it = walls_vec.begin(); it != walls_vec.end(); ++it) {
		it->draw(this->canvas);
	}

	if (exit_point != NULL)
	{
		exit_point->Draw();
	}
};

/*void Game::erase_all()
{
	size_t i;

	for (i = 0; i < num_of_items; i++)
	{
		items[i]->EraseDrawing();
	}

	for (i = 0; i < num_of_ships; i++)
	{
		ships[i]->EraseDrawing();
	}

	for (i = 0; i < num_of_walls; i++)
	{
		walls[i]->EraseDrawing();
	}

	if (exit_point != NULL)
	{
		exit_point->EraseDrawing();
	}
}*/

void Game::Run()
{
	this->draw_all();
	// esc (Ascii 27) ends the loop
	while (!_kbhit() || _getch() != ESC)
	{
		for (vector<Item>::iterator it = items_vec.begin(); it != items_vec.end(); ++it) {
			it->advance_falling(this);
		}

		//for each ship do IsAtExitPoint and if so - quit the loop

		Sleep(150);
	}


	//this->erase_all();

	gotoxy(UPPER_X / 2, UPPER_Y / 2);
	cout << "GAME OVER!";
}