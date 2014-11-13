#ifndef _WALL_H_
#define _WALL_H_

using namespace std;

typedef struct
{
	int x;
	int y;
}Wall_location_t;

typedef struct
{
	int size_lines;
	int size_columns;
}Wall_size_t;

class Wall
{
private:
	Wall_location_t location;
	Wall_size_t size;

public:
	Wall(Wall_location_t wall_stating_location, Wall_size_t wall_size)
	{
		location = wall_stating_location;
		size = wall_size;
	};

	void Draw()
	{

	};
};
#endif