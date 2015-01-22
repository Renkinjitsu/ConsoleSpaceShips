#include "Bomb.h"

#include "GameConfig.h"

unsigned Bomb::getExplosionDistance()
{
	return Bomb::_explosionDistance;
}

Bomb::Bomb(const Point & position) : GameObject(GameConfig::TEXTURES_BOMB , true)
{
	this->setPoints(position);
}
