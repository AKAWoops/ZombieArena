#pragma once
#include <SFML/Graphics.hpp>

using namespace sf;

class Player
{
private:
	const float START_SPEED = 200;
	const float START_HEALTH = 100;
	// where is the player
	Vector2f m_Position;
	// the sprite
	Sprite m_Sprite;
	//add a texture
	// !!additions to come!!
	Texture m_Texture;
	// screen resolution what is it
	Vector2f m_Resolution;
	// what size is the current arena
	IntRect m_Arena;
	// How big is each tile of the arena
	int m_TileSize;
	// what direction(s) is the player moving
	bool m_UpPressed;
	bool m_DownPressed;
	bool m_LeftPressed;
	bool m_RightPressed;
	// how much health does player have
	int m_Health;
	// what is the max health the player can have hard limit
	int m_MaxHealth;
	// when was the player hit last
	Time m_LastHit;
	// Speed in pixels per seccond
	float m_Speed;

	//===Public functions below===//
public:
	Player();
	void spawn(IntRect arena, Vector2f resolution, int tileSize);
	// call thi at the end of every game dont forget to add this end as well
	void resetPlayerStats();

	//==hits==//
	// handle the player getting hit by zombie
	bool hit(Time timeHit);
	// how long ago was the player last hit
	Time getLastHitTime();
	// where is the player
	FloatRect getPosition();
	// where is the center of the player
	Vector2f getCenter();
	// what angle is the player facing
	float getRotation();
	// send a copy of the sprite to the main function
	Sprite getSprite();
	// the next 4 functions move the player
	void moveLeft();
	void moveRight();
	void moveUp();
	void moveDown();
	// stop the player moving ina  specific direction
	void stopLeft();
	void stopRight();
	void stopUp();
	void stopDown();
	// we will call this function once every frame
	void update(float elapsedTime, Vector2i mousePosition);
	// Give the player some health
	// Give the player a speed boost
	void upgradeSpeed();
	// give the playersome health
	void upgradeHealth();
	// Increase the max' health the player can have
	void increaseHealthLevel(int amount);
	// how much health has the player currently got???
	int getHealth();
};