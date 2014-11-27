
#ifndef DEFINITIONS_H_
#define DEFINITIONS_H_

typedef enum
{
	DIRECTION_UP,
	DIRECTION_DOWN,
	DIRECTION_LEFT,
	DIRECTION_RIGHT
}game_direction_e;

typedef struct
{
	bool is_carried_on_ship;
	bool explosion_allowed;
}game_move_flags_t;

#endif