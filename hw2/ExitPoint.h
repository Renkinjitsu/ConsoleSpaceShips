#ifndef _EXITPOINT_H_
#define _EXITPOINT_H_
#include "ExitPoint.h"
#include "io_utils.h"
using namespace std;

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

	void Draw()
	{
		gotoxy(location.x, location.y);
		cout << "X";
	};

	void EraseDrawing()
	{
		gotoxy(location.x, location.y);
		cout << " ";
	};
};
#endif