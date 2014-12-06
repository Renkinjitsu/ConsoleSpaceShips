#include "Game.h"

#include "Item.h"
#include "SmallShip.h"
#include "BigShip.h"
#include "Wall.h"
#include "ExitPoint.h"

int main()
{
	Canvas canvas;

	SmallShip smallShip(2, 2);
	BigShip bigShip(18, 12);

	Game game(canvas, smallShip, bigShip);

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
	item2.setLocations(13, 7);
	item2.setLocations(13, 6);
	item2.setLocations(13, 5);
	item2.setLocations(13, 4);
	game.addGameObject(&item2);

	Item item3('3');
	item3.setLocations(6, 8);
	item3.setLocations(6, 9);
	game.addGameObject(&item3);

	Item item4('4');
	item4.setLocations(69, 7);
	item4.setLocations(69, 8);
	item4.setLocations(69, 9);
	item4.setLocations(70, 7);
	item4.setLocations(70, 8);
	item4.setLocations(70, 9);
	game.addGameObject(&item4);

	Item item5('5');
	item5.setLocations(15, 20);
	game.addGameObject(&item5);

	Item item6('6');
	item6.setLocations(15, 5);
	game.addGameObject(&item6);

	Item item7('7');
	item7.setLocations(41, 20);
	game.addGameObject(&item7);

	Item item8('8');
	item8.setLocations(42, 5);
	item8.setLocations(42, 6);
	item8.setLocations(42, 7);
	item8.setLocations(41, 5);
	item8.setLocations(41, 6);
	game.addGameObject(&item8);

	ExitPoint exitPoint(68, 6);
	game.addGameObject(&exitPoint);

	const unsigned vertWallWidth = 8;
	const unsigned horiWallHeight = 20;


	const unsigned secondWallWidth = 25;
	const unsigned ThirdWallWidth = 19;

	const unsigned vertSecondWallHeight = 7;
	const unsigned vertThirdWallHeight = 16;
	const unsigned vertFourthWallHeight = 15;
	const unsigned vertFifthWallHeight = 3;
	const unsigned vertSixthWallHeight = 20;

	std::vector<Wall *> walls;

	//Allocate vertical border walls
	walls.push_back(new Wall(1, 4, 8, 1));
	walls.push_back(new Wall(Canvas::getWidth() - vertWallWidth, 0, vertWallWidth, 5));
	walls.push_back(new Wall(Canvas::getWidth() - vertWallWidth, Canvas::getHeight() - 1, vertWallWidth, 1));


	//Allocate horizontal border walls
	walls.push_back(new Wall(Canvas::getWidth() - 1, 1, 1, horiWallHeight));
	walls.push_back(new Wall(0, Canvas::getHeight() - 1 - horiWallHeight, 1, horiWallHeight));
	walls.push_back(new Wall(Canvas::getWidth() - 1, Canvas::getHeight() - 1 - horiWallHeight, 1, horiWallHeight));

	//Allocate non-border walls
	walls.push_back(new Wall(66, 6, 2, 2));

	walls.push_back(new Wall(8, 0, 1, vertSecondWallHeight));
	walls.push_back(new Wall(8, 8, 1, vertThirdWallHeight));
	walls.push_back(new Wall(13, 8, 1, vertFourthWallHeight));
	walls.push_back(new Wall(13, 1, 1, vertFifthWallHeight));
	walls.push_back(new Wall(64, 4, 1, vertSixthWallHeight));

	walls.push_back(new Wall(16, 4, secondWallWidth, 1));
	walls.push_back(new Wall(16 + secondWallWidth + 4, 4, ThirdWallWidth, 1));

	//Item 3 ramp
	walls.push_back(new Wall(5, 4, 2, 3));
	walls.push_back(new Wall(7, 4, 1, 2));

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