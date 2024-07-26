#include "Bullet.h"

// the constructor

Bullet::Bullet()
{
	m_BulletShape.setSize(sf::Vector2f(2, 2));
}

void Bullet::shoot(float startX, float startY, float TargetX, float TargetY)
{
	// how to keep track of the bullet
	m_Inflight = true;
	m_Position.x = startX;
	m_Position.y = startY;
	// calculate the gradient of the flight path would be startx - targety devided by starty - targety
	float gradient = (startX - TargetY) / (startY - TargetY);
	// any gradient less than 1 needs to be negative

	if (gradient < 0)// try with one if zero does not work 
	{
		gradient *= -1;
	}
	// calculate the speed ratio between x and y
	float ratioXY = m_BulletSpeed / (1 + gradient);
	// set the "speed" horizontally and vertically
	m_BulletDistanceY = ratioXY;
	m_BulletDistanceX = ratioXY * gradient;

	// point the bullet in the right direction
	if (TargetX < startX)
	{
		m_BulletDistanceX *= -1;
	}
	if (TargetY < startY)
	{
		m_BulletDistanceY *= -1;
	}

	// set max range of 1000 pixels
	float range = 1000;
	m_MinX = startX - range;
	m_MaxX = startX + range;
	m_MinY = startY - range;
	m_MaxY = startY + range;

	// Position the bullet ready to be drawn
	m_BulletShape.setPosition(m_Position);
}