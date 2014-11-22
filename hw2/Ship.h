#ifndef SHIP_H_
#define SHIP_H_

#include "GameObject.h"

class Ship : public GameObject
{
private:
	const char texture;

protected:
	unsigned width;
	unsigned height;

public:
	Ship(unsigned xPositin, unsigned yPosition,
		unsigned width, unsigned height,
		char texture);

	bool canMoveDown(Game * game_screen);
	bool canMoveUp(Game * game_screen);
	bool canMoveLeft(Game * game_screen);
	bool canMoveRight(Game * game_screen);
	bool MoveDown(Game * game_screen);
	bool MoveUp(Game * game_screen);
	bool MoveLeft(Game * game_screen);
	bool MoveRight(Game * game_screen);

	void draw(Canvas & canvas);
	void EraseDrawing();
};

#endif /* SHIP_H_ */