#include "ExitPoint.h"

#include "GameConfig.h"

ExitPoint::ExitPoint(const std::vector<Point> & points) : GameObject(GameConfig::TEXTURES_EXIT, points, false)
{
	//Nothing to do
}
