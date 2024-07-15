#include "ZombieArena.h"
#include "Zombie.h"

Zombie* createHorde(int numZombies, IntRect arena)
{
	Zombie* zombies = new Zombie[numZombies];
	int maxY = arena.height - 20;
	int minY = arena.top + 20;
	int maxX = arena.width - 20;
	int minX = arena.left + 20;
	for (int i = 0; i < numZombies; i++)
	{

		// which side should the zombie spawn
		srand((int)time(0) * i);
		int side = (rand() % 4);
		float x, y;
		switch (side)
		{
		case 0:
			// left side
			x = minX;
			y = (rand() % maxY) + minY;
			break;
		case 1:
			// right side
			x = maxX;
			y = (rand() % maxY) + minY;
			break;
		case 2:
			// Top side
			x = (rand() % maxX) + minX;
			y = minY;
			break;
		case 3:
			// Bottom side
			x = (rand() % maxX) + minX;
			y = minY;
			break;
		}
		// Bloater, crawler or a runner
		srand((int)time(0) * i * 2);
		int type = (rand() % 3);
		// Spawn the new zombie into the array
		zombies[i].spawn(x, y, type, i);
	}
	return zombies;
}