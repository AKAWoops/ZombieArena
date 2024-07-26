#pragma once

#include <SFML/Graphics.hpp>
using namespace sf;

class Bullet
{
	// where is my bullet?
	Vector2f m_Position;
	// what each bullet will look like
	RectangleShape m_BulletShape;
	// is the bullet currently whizzing/flying through the air?
	bool m_Inflight = false;
	// how fast wil bullet be traveling in air?
	float m_BulletSpeed = 1000;
	// what fraction of 1 pixel does a bullet travel
	// horizontally aand vertically on each frame ?
	// i want these to be derived from bulletspeed

	float m_BulletDistanceX;
	float m_BulletDistanceY;

	// set some boundaies for the bullet not to travel
	float m_MaxX;
	float m_MinX;
	float m_MaxY;
	float m_MinY;

	// public function prototypes will go here below
public:
	// das constructor
	Bullet();
	// stop the nullet
	void stop();
	// Returns the value of M_InFlight
	bool isInFlight();
	// launch a new bullet pew pew
	void shoot(float startX, float startY,
		float xTarget, float yTarget);
	// tell the calling code where he bullet will be in the world
	FloatRect getPosition();
	// Return the actual shape (for drawing the bullet)
	RectangleShape getShape();
	// update the bulet each frame
	void update(float elapsedTime);
};