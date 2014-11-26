#ifndef _EXITPOINT_H_
#define _EXITPOINT_H_
#include "ExitPoint.h"
#include "Game.h"
#include "io_utils.h"
using namespace std;

class Ship;

/*typedef struct
{
	int x;
	int y;
}ExitPoint_location_t;*/

class ExitPoint : public GameObject
{
private:
public:
	ExitPoint(Object_location_t exit_p_location) : GameObject(OBJECT_EXITP, exit_p_location.x, exit_p_location.y)
	{
		//this->set_locations(exit_p_location.x, exit_p_location.y);
		//location = exitpoint_start_location;
	};

	void draw(Canvas & canvas)
	{
		Object_location_t location = get_location();
		canvas.setCharacter(location.x, location.y, 'X');
	};

	void EraseDrawing(Canvas & canvas)
	{
		Object_location_t location = get_location();
		canvas.setCharacter(location.x, location.y, ' ');
	};

	bool IsAtExitPoint(const Object_location_t & loc);
};

#endif