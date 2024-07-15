#pragma once
#include <SFML/Graphics.hpp>
#include "Zombie.h"

using namespace sf;

int createBackGround(VertexArray& rVA, IntRect arena);
Zombie* createHorde(int numZombies, IntRect arena);