#include "io_utils.h"
#include "Game.h"

#define ESC 27

void Game::draw_all()
{
	size_t i;

	for (i = 0; i < num_of_items; i++)
	{
		items[i]->draw(this->canvas);
	}

	for (i = 0; i < num_of_ships; i++)
	{
		ships[i]->draw(this->canvas);
	}

	for (i = 0; i < num_of_walls; i++)
	{
		walls[i]->draw(this->canvas);
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
		for (size_t i = 0; i < this->get_num_of_items(); i++)
		{
			this->get_item(i)->advance_falling(this);
		}

		//for each ship do IsAtExitPoint and if so - quit the loop

		Sleep(150);
	}


	//this->erase_all();

	gotoxy(UPPER_X / 2, UPPER_Y / 2);
	cout << "GAME OVER!";
}