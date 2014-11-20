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
}Item_location_t;

typedef enum
{
	SINGLE_LOCATION_TEXTURE,
	MULTI_LOCATION_TEXTURE
}texture_e;

class GameObject
{
private:
	Item_location_t start_location;
	vector<Item_location_t> locations;

	texture_e texture_type;

public:
	//Single location
	GameObject(unsigned x = 0, unsigned y = 0);

	//Multi location
	GameObject(vector<Item_location_t> & location);

	void set_locations(unsigned x = 0, unsigned y = 0);
	void set_locations(vector<Item_location_t> & locatio);

	vector<Item_location_t> & GameObject::get_locations();

	//only on single location
	unsigned getXstart();
	unsigned getYstart();

	void setPosition(unsigned x, unsigned y);

	void moveUp();
	virtual bool moveDown(Game * screen);
	void moveLeft();
	void moveRight();

	virtual void draw(Canvas & canvas) = 0;
};

#endif /* GAME_OBJECT_H_ */