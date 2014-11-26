#ifndef SHIP_H_
#define SHIP_H_
#include "Definitions.h"
#include "GameObject.h"

class Ship : public GameObject
{
private:
	const char texture;
	bool is_alive;

protected:
	unsigned width;
	unsigned height;

public:
	Ship(unsigned xPositin, unsigned yPosition,
		unsigned width, unsigned height,
		char texture);

	bool IsShipAlive();
	void SetShipAlive(bool is_alive);

	void KillShip();

	bool canMoveDown(Game * game_screen, game_move_flags_t flags);
	bool canMoveUp(Game * game_screen, game_move_flags_t flags);
	bool canMoveLeft(Game * game_screen, game_move_flags_t flags);
	bool canMoveRight(Game * game_screen, game_move_flags_t flags);
	bool MoveDown(Game * game_screen, game_move_flags_t flags);
	bool MoveUp(Game * game_screen, game_move_flags_t flags);
	bool MoveLeft(Game * game_screen, game_move_flags_t flags);
	bool MoveRight(Game * game_screen, game_move_flags_t flags);

	void draw(Canvas & canvas);
	void EraseDrawing(Canvas & canvas);
};

#endif /* SHIP_H_ */