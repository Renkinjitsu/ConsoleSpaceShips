#ifndef GAME_OBJECT_H_
#define GAME_OBJECT_H_

#include "Canvas.h"
#include <vector>
using namespace std;

class Game;

typedef struct
{
	int x;
	int y;
}Object_location_t;

typedef enum
{
	SINGLE_LOCATION_TEXTURE,
	MULTI_LOCATION_TEXTURE
}texture_e;

typedef enum
{
	OBJECT_ITEM,
	OBJECT_SHIP,
	OBJECT_WALL,
	OBJECT_EXITP
}Object_type_e;

class GameObject
{
private:
	Object_location_t start_location;
	vector<Object_location_t> locations;

	texture_e texture_type;
	Object_type_e object_type;

public:
	//Single location
	GameObject(Object_type_e object_type, unsigned x = 0, unsigned y = 0);

	//Multi location
	GameObject(vector<Object_location_t> & location, Object_type_e object_type);

	Object_type_e get_type()
	{
		return object_type;
	};

	void set_locations(unsigned x = 0, unsigned y = 0);
	void set_locations(vector<Object_location_t> & locatio);

	void clear_locations();

	vector<Object_location_t> & GameObject::get_locations();
	Object_location_t GameObject::get_location();

	//only on single location
	unsigned getXstart();
	unsigned getYstart();

	void setPosition(unsigned x, unsigned y);

	void moveUp();
	bool moveDown(Game * screen);
	void moveLeft();
	void moveRight();

	virtual void draw(Canvas & canvas) = 0;
};

#endif /* GAME_OBJECT_H_ */