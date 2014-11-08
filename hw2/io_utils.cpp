
#include "io_utils.h"

using namespace std;

#ifndef WINDOWS
void gotoxy(int x, int y) {}
int _getch(void){ return 0; }
int _kbhit(void){ return 0; }
void Sleep(unsigned long){}

#else
void gotoxy(int x, int y)
{
	HANDLE hConsoleOutput;
	COORD dwCursorPosition;
	cout.flush();
	dwCursorPosition.X = x;
	dwCursorPosition.Y = y;
	hConsoleOutput = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(hConsoleOutput, dwCursorPosition);
}
#endif