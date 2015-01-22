#ifndef BOMB_H_
#define BOMB_H_

#include "GameObject.h"

class Bomb : public GameObject
{
private:
	static const unsigned _explosionDistance = 4;

public:
	static unsigned getExplosionDistance();

	Bomb(const Point & position);
};

#endif