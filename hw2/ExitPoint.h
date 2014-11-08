#ifndef _EXITPOINT_H_
#define _EXITPOINT_H_

typedef struct
{
	int x;
	int y;
}ExitPoint_location_t;

typedef struct
{
	int size_lines;
	int size_columns;
}ExitPoint_size_t;

class ExitPoint
{
private:
	ExitPoint_location_t location;
	ExitPoint_size_t size;
public:
	ExitPoint(ExitPoint_location_t exitpoint_start_location, ExitPoint_size_t exitpoint_size)
	{
		size = exitpoint_size;
		location = exitpoint_start_location;
	};
};
#endif