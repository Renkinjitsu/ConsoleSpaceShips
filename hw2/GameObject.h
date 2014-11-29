#ifndef GAME_OBJECT_H_
#define GAME_OBJECT_H_

#include "Canvas.h"

#include <vector>
#include "Direction.h"
#include "Point.h"

class GameObject
{
private:
	std::vector<Point> _locations;
	char _texture;

	bool _isPushable;

protected:
	void clearLocations();
	std::vector<Point> getLocations();

public:
	GameObject(char texture, bool isPushable);
	GameObject(char texture, unsigned x, unsigned y, bool isPushable);
	GameObject(char texture, std::vector<Point> & location, bool isPushable);

	void setLocations(unsigned x, unsigned y);
	void setLocations(const Point & point);
	void setLocations(std::vector<Point> & locations);

	bool isBlockedBy(const GameObject & other, Direction from) const;
	bool isPushable();
	bool collidesWith(const GameObject & other) const;

	unsigned getMass();

	void move(Direction direction);

	void draw(Canvas & canvas);
};

#endif /* GAME_OBJECT_H_ */