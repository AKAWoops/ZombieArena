#pragma once

#include <SFML/Graphics.hpp>

using namespace sf;

class Pickup
{
private:
	// Start values for the health pickups
	const int HEALTH_START_VALUE = 50;
	const int AMMO_START_VALUE = 12;
	const int START_WAIT_TIME = 10;
	const int START_SECCONDS_TO_LIVE = 5;

	// The sprite that represents this pickup
	Sprite m_Sprite;
	// The arena it exists in
	IntRect m_Arena;
	// How much is this pickup worth?
	// 1 = health, 2 = ammo
	int m_Type;
	// Handle spawning and disappearing
	bool m_Spawned;
	float m_SecondsSinceSpawn;
	float m_SecondsSinceDeSpawn;
	float m_SecondsToLive;
	float m_SecondsToWait;

	// public prototype variables go here later thinking of what to put in
public:

};