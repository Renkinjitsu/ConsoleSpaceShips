

#include "io_utils.h"

using namespace std;

#define ESC 27

int main()
{
	system("cls");

	int x = 1;
	int y = 1;

	// esc (Ascii 27) ends the loop
	while (!_kbhit() || _getch() != ESC)
	{
		gotoxy(x, y);
		cout << "*" << endl;
		Sleep(50);
		gotoxy(x, y);
		cout << " " << endl;
		++x;
		if (x>79) { x = 1; }
		++y;
		if (y>20) { y = 1; }
	}
	return 0;
}