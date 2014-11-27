#ifndef GAME_OBJECT_H_
#define GAME_OBJECT_H_

#include <vector>

#include "Canvas.h"

#include "Definitions.h"

class Game;

class Object_location_t
{
public:
	int x;
	int y;

public:
	void move(Direction direction)
	{
		switch(direction)
		{
			case DIRECTION_UP:
			{
				y = (y + 1) % Canvas::getHeight();
			}
			break;

			case DIRECTION_DOWN:
			{
				y = (y + Canvas::getHeight() - 1) % Canvas::getHeight();
			}
			break;

			case DIRECTION_LEFT:
			{
				x = (x + Canvas::getWidth() - 1) % Canvas::getWidth();
			}
			break;

			case DIRECTION_RIGHT:
			{
				x = (x + 1) % Canvas::getWidth();
			}
			break;
		}
	}

	bool equals(const Object_location_t & other)
	{
		return (this->x == other.x && this->y == other.y);
	}
};

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
	std::vector<Object_location_t> locations;

	texture_e texture_type;
	Object_type_e object_type;

public:
	//Single location
	GameObject(Object_type_e object_type, unsigned x = 0, unsigned y = 0);

	//Multi location
	GameObject(std::vector<Object_location_t> & location, Object_type_e object_type);

	Object_type_e get_type()
	{
		return object_type;
	};

	void set_locations(unsigned x = 0, unsigned y = 0);
	void set_locations(std::vector<Object_location_t> & locatio);

	void clear_locations();

	std::vector<Object_location_t> & GameObject::get_locations();
	Object_location_t GameObject::get_location();

	//only on single location
	unsigned getXstart();
	unsigned getYstart();

	void setPosition(unsigned x, unsigned y);

	bool isBlockedBy(const GameObject & other, Direction from);

	virtual void draw(Canvas & canvas) = 0;
};

#endif /* GAME_OBJECT_H_ */