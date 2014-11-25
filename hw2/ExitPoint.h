#ifndef _EXITPOINT_H_
#define _EXITPOINT_H_
#include "ExitPoint.h"
#include "Game.h"
#include "io_utils.h"
using namespace std;

class Ship;

typedef struct
{
	int x;
	int y;
}ExitPoint_location_t;

class ExitPoint
{
private:
	ExitPoint_location_t location;
public:
	ExitPoint(ExitPoint_location_t exitpoint_start_location)
	{
		location = exitpoint_start_location;
	};

	bool IsAtExitPoint(Ship * ship);

	void Draw()
	{
		gotoxy(location.x, location.y);
		cout << "X";
	};

	void EraseDrawing(Canvas & canvas)
	{
		canvas.setCharacter(location.x, location.y, ' ');
	};
};
#endif