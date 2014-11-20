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
	SQUARE_TEXTURE,
	MULTI_LOCATION_TEXTURE
}texture_e;

class GameObject
{
private:
	/*unsigned x; //The amount of characters from the left
	unsigned y; //The amount of characters from the bottom*/

	Item_location_t start_location;

	vector<Item_location_t> locations;

	texture_e texture_type;

public:
	GameObject(unsigned x = 0, unsigned y = 0);
	GameObject(vector<Item_location_t> & location);

	void set_locations(unsigned x = 0, unsigned y = 0);
	void set_locations(vector<Item_location_t> & locatio);

	vector<Item_location_t> & GameObject::get_locations();


	unsigned getXstart();
	unsigned getYstart();

	void setPosition(unsigned x, unsigned y);

	void moveUp();
	virtual bool moveDown(Game * screen);
	void moveLeft();
	void moveRight();

	bool GameObject::IsUpperTouchingDowner(GameObject * downer, GameObject * upper);

	virtual void draw(Canvas & canvas) = 0;
};

#endif /* GAME_OBJECT_H_ */