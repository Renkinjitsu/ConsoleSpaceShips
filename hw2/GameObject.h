#ifndef GAME_OBJECT_H_
#define GAME_OBJECT_H_

#include "Canvas.h"

class GameObject
{
private:
	unsigned x; //The amount of characters from the left
	unsigned y; //The amount of characters from the bottom

public:
	GameObject(unsigned x = 0, unsigned y = 0);

	unsigned getX();
	unsigned getY();

	void setPosition(unsigned x, unsigned y);

	void moveUp();
	void moveDown();
	void moveLeft();
	void moveRight();

	virtual void draw(Canvas & canvas) = 0;
};

#endif /* GAME_OBJECT_H_ */