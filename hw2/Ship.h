#ifndef _SHIP_H_
#define _SHIP_H_

typedef struct
{
	int x;
	int y;
}Ship_location_t;

typedef struct
{
	int size_lines;
	int size_columns;
}Ship_size_t;

class Ship
{
private:
	Ship_location_t location;
	Ship_size_t size;


public:
	Ship_location_t get_ship_location()
	{
		return location;
	};

	Ship(Ship_size_t ship_size, Ship_location_t ship_starting_location)
	{
		size = ship_size;
		location = ship_starting_location;
	};


};

#endif