#include "Game.h"

#include "Item.h"
#include "SmallShip.h"
#include "BigShip.h"
#include "Wall.h"
#include "ExitPoint.h"

int main()
{
	Game game;

	Item item1('1');
	item1.setLocations(10, 5);
	item1.setLocations(11, 5);
	item1.setLocations(12, 5);
	item1.setLocations(12, 6);
	item1.setLocations(12, 7);
	item1.setLocations(12, 8);
	item1.setLocations(11, 8);
	item1.setLocations(10, 8);
	game.addGameObject(&item1);

	Item item2('2');
	item2.setLocations(10, 6);
	item2.setLocations(11, 6);
	item2.setLocations(10, 7);
	item2.setLocations(11, 7);
	game.addGameObject(&item2);

	Item item3('3');
	item3.setLocations(41, 21);
	item3.setLocations(42, 22);
	item3.setLocations(43, 23);
	item3.setLocations(44, 24);
	game.addGameObject(&item3);

	Item item4('4');
	item4.setLocations(29, 7);
	game.addGameObject(&item4);

	Item item5('5');
	item5.setLocations(15, 20);
	game.addGameObject(&item5);

	Item item6('6');
	item6.setLocations(15, 5);
	game.addGameObject(&item6);

	Item item7('7');
	item7.setLocations(69, 20);
	game.addGameObject(&item7);

	ExitPoint exitPoint(68, 6);
	game.addGameObject(&exitPoint);

	SmallShip smallShip(12, 12);
	game.addGameObject(&smallShip);

	BigShip bigShip(18, 12);
	game.addGameObject(&bigShip);

	const unsigned wallWidth = 8;
	const unsigned wallHeight = 5;

	std::vector<Wall *> walls;

	//Allocate vertical border walls
	walls.push_back(new Wall(0, 0, wallWidth, 1));
	walls.push_back(new Wall(0, Canvas::getHeight() - 1, wallWidth, 1));
	walls.push_back(new Wall(Canvas::getWidth() - wallWidth, 0, wallWidth, 1));
	walls.push_back(new Wall(Canvas::getWidth() - wallWidth, Canvas::getHeight() - 1, wallWidth, 1));

	//Allocate horizontal border walls
	walls.push_back(new Wall(0, 1, 1, wallHeight));
	walls.push_back(new Wall(Canvas::getWidth() - 1, 1, 1, wallHeight));
	walls.push_back(new Wall(0, Canvas::getHeight() - 1 - wallHeight, 1, wallHeight));
	walls.push_back(new Wall(Canvas::getWidth() - 1, Canvas::getHeight() - 1 - wallHeight, 1, wallHeight));

	//Allocate non-border walls
	walls.push_back(new Wall(66, 6, 2, 2));

	//Register walls
	for(std::vector<Wall *>::iterator wallIter = walls.begin(); wallIter != walls.end(); ++wallIter)
	{
		game.addGameObject(*wallIter);
	}

	game.run();

	for(std::vector<Wall *>::iterator wallIter = walls.begin(); wallIter != walls.end(); ++wallIter)
	{
		delete *wallIter;
	}

	return 0;
}