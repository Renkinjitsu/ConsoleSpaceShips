#ifndef GAME_OBJECT_H_
#define GAME_OBJECT_H_

#include "Canvas.h"

#include <vector>

#include "Point.h"

class GameObject
{
private:
	std::vector<Point> _points;
	char _texture;

	bool _isPushable;

protected:
	void clearPoints();
	std::vector<Point> getPoints() const;

public:
	GameObject(char texture, bool isPushable);
	GameObject(char texture, const Point & position, bool isPushable);
	GameObject(char texture, const std::vector<Point> & points, bool isPushable);

	void setPoints(unsigned x, unsigned y);
	void setPoints(const Point & point);
	void setPoints(const std::vector<Point> & points);

	char getTexture() const;
	void setTexture(const char texture);

	bool isBlockedBy(const GameObject & other, const Point & fromDirection) const;
	bool isPushable() const;
	bool collidesWith(const GameObject & other) const;
	bool isTouching(const GameObject & other) const;

	unsigned getMass() const;
	Point getTopLeft() const;
	unsigned getStepDistance(const Point & point) const;
	unsigned getStepDistance(const GameObject & other) const;
	Point getClosestStepDistancePoint(const GameObject & other) const;

	void move(const Point & offset);

	void draw(Canvas & canvas);
};

#endif /* GAME_OBJECT_H_ */