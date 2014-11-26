#include "ExitPoint.h"
#include "Game.h"

bool ExitPoint::IsAtExitPoint(const Object_location_t & loc)
{
	Object_location_t location = get_location();
	if (loc.x == location.x && loc.y == location.y)
	{
		return true;
	}

	return false;
}