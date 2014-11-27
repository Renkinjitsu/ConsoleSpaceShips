
#ifndef DEFINITIONS_H_
#define DEFINITIONS_H_

enum Direction
{
	DIRECTION_UP,
	DIRECTION_DOWN,
	DIRECTION_LEFT,
	DIRECTION_RIGHT,
	DIRECTION_NONE
};

typedef struct
{
	bool is_carried_on_ship;
}game_move_flags_t;

#endif