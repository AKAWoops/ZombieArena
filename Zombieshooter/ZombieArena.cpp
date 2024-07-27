#include "ZombieArena.h"

#include <SFML/Graphics.hpp>

#include "Bullet.h"
#include "Player.h"
#include "TextureHolder.h"

using namespace sf;

int main()
{
	// here is the instance textureHolder
	TextureHolder holder;
	// this game has 4 states and will be in one fo four states
	enum class State
	{
		PAUSED, LEVELING_UP,
		GAME_OVER, PLAYING
	};
	//hello
	// start with the GAME_OVER state
	State state = State::GAME_OVER;
	// Get the screen resolution and
	// create an SFML window
	Vector2f resolution;
	resolution.x =
		VideoMode::getDesktopMode().width;
	resolution.y =
		VideoMode::getDesktopMode().height;
	RenderWindow window(
		VideoMode(resolution.x, resolution.y),
		"Zombie Arena", Style::Fullscreen);
	// creatye an sfml view for the main action
	View mainView(sf::FloatRect(0, 0, 
		resolution.x, resolution.y));
	// here is our clock for timing everything
	Clock clock;
	// how long has the PLAYING STATE been up for and active
	Time gameTimeTotal;
	// Where is the mouse in
	// relation to the world coordinates
	Vector2f mouseWorldPosition;
	// where is the mouse in relation
	// to the screen coordinates
	Vector2i mouseScreenPosition;
	// create an instance of the Player class
	Player player;
	// the boundaries of the arena
	IntRect arena;
	// Create the background
	VertexArray background;
	// Load the texture for our background vertex array
	//Texture textureBackground;
	//textureBackground.loadFromFile("graphics/background_sheet.png");
	// Load Texture for our background vertex array
	Texture textureBackground = TextureHolder::GetTexture(
		"graphics/background_sheet.png");


	// Prepare for a horde of zombies
	int numZombies;
	int numZombiesAlive;
	Zombie* zombies = NULL;
	// 100 bullets should do
	Bullet bullets[100];
	int currentBullet = 0;
	int bulletSpare = 24;
	int bulletsInClip = 6;
	int clipSize = 6;
	float fireRate = 1;
	// when was is the fire buttoion last pressed
	Time lastPressed;

	// ==== THE MAIN GAME LOOP ==== //
	while (window.isOpen())
	{
		// -== INPUT HANDLING ==- //
		/*
		 ************
		 HANDLE INPUT
		 ************
		 */
		 // Handle events by polling
		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::KeyPressed)
			{
				//Pause game whle playing
				if (event.key.code == Keyboard::Return &&
					state == State::PLAYING)
				{
					state = State::PAUSED;
				}
				// Restart while paused
				else if (event.key.code == Keyboard::Return &&
					state == State::PAUSED)
				{
					state = State::PLAYING;
					// reset the clock so there is not a frame jump
					clock.restart();
				}
				// Start a new game while in GAME_OVER state
				else if (event.key.code == Keyboard::Return &&
					state == State::GAME_OVER)
				{
					state = State::LEVELING_UP;
				}
				if (state == State::PLAYING)
				{
					// Reloading //
					if (event.key.code == Keyboard::R)
					{
						if (bulletSpare >= clipSize)
						{
							// plenty of bullets... reload
							bulletsInClip = clipSize;
							bulletSpare -= clipSize;
						}
						else if (bulletSpare > 0)
						{
							// Only few bullets left
							bulletsInClip = bulletSpare;
							bulletSpare = 0;
						}
						else
						{
							/// more to come need to think :-)
						}
					}
				}
			}
		}// End of event polling

		// Handle the player quitting
		if (Keyboard::isKeyPressed(Keyboard::Escape))
		{
			window.close();
		}
		// Handle WASD while playing
		if (state == State::PLAYING)
		{
			//handle the pressing and releasing of the WASD keys
			if (Keyboard::isKeyPressed(Keyboard::W))
			{
				player.moveUp();
			}
			else
			{
				player.stopUp();
			}
			if (Keyboard::isKeyPressed(Keyboard::S))
			{
				player.moveDown();
			}
			else
			{
				player.stopDown();
			}
			if (Keyboard::isKeyPressed(Keyboard::A))
			{
				player.moveLeft();
			}
			else
			{
				player.stopLeft();
			}
			if (Keyboard::isKeyPressed(Keyboard::D))
			{
				player.moveRight();
			}
			else
			{
				player.stopRight();
			}
			//===================================================//
			//=== Fire bullet here ===//
		// Fire a bullet
			if (Mouse::isButtonPressed(sf::Mouse::Left))
			{
				if (gameTimeTotal.asMilliseconds()
					- lastPressed.asMilliseconds()
				> 1000 / fireRate && bulletsInClip > 0)
				{
					// pass the center of the player and the center
					// of the cross hair to the shoot function
					bullets[currentBullet].shoot(
						player.getCenter().x, player.getCenter().y,
						mouseWorldPosition.x, mouseWorldPosition.y);
					currentBullet++;
					if (currentBullet > 99)
					{
						currentBullet = 0;
					}
					lastPressed = gameTimeTotal;
					bulletsInClip--;
				} 
			}// End of bullet fire
		}// End WASD while playing
		//===============================================================//
		//=== HANDLE the LEVELING up STATE ===//
		if (state == State::LEVELING_UP)
		{
			// handle the player LEVELING UP
			if (event.key.code == Keyboard::Num1)
			{
				state = State::PLAYING;
			}
			if (event.key.code == Keyboard::Num2)
			{
				state = State::PLAYING;
			}
			if (event.key.code == Keyboard::Num3)
			{
				state = State::PLAYING;
			}
			if (event.key.code == Keyboard::Num4)
			{
				state = State::PLAYING;
			}
			if (event.key.code == Keyboard::Num5)
			{
				state = State::PLAYING;
			}
			if (event.key.code == Keyboard::Num6)
			{
				state = State::PLAYING;
			}

			if (state == State::PLAYING)
			{
				// prepare the level
				// we will modify the next two lines later
				arena.width = 500;
				arena.height = 500;
				arena.left = 0;
				arena.top = 0;
				// pass the vertex array by reference
				// to the creatBackground function
				int tileSize = createBackGround(background, arena);
				// we will modify this line of code later
				//int tileSize = 50;
				// spawn the player int he middle of the arena
				player.spawn(arena, resolution, tileSize);

				// create a horde of zombies //
				numZombies = 2000;
				// delete the previously allocated memory (if it exists)
				delete[] zombies;
				zombies = createHorde(numZombies, arena);
				numZombiesAlive = numZombies;

				// reset clock so there is not a frame jump
				clock.restart();
			}
		} // END of LEVELING UP

		// -== FRAME UPDATE ==- //
		/*
		 ****************
		 UPDATE THE FRAME
		 ****************
		 */
		if (state == State::PLAYING)
		{
			// Update the delta time
			Time dt = clock.restart();

			// Update the total game time
			gameTimeTotal += dt;

			// Make a fraction of 1 from delta time
			float dtAsSeconds = dt.asSeconds();
			// Where is the mouse pointer
			mouseScreenPosition = Mouse::getPosition();
			// convert mouse position to world
			// based coordinates of mainView
			mouseWorldPosition = window.mapPixelToCoords(
				Mouse::getPosition(), mainView);
			// Update tje player
			player.update(dtAsSeconds, Mouse::getPosition());
			// Make a note of the players new position
			Vector2f playerPosition(player.getCenter());

			// Make the view centre
			// the around player
			mainView.setCenter(player.getCenter());

			// Loop through each Zombie and update them all
			for (int i = 0; i < numZombies; i++)
			{
				if (zombies[i].isAlive())
				{
					zombies[i].update(dt.asSeconds(), playerPosition);
				}
			}
			// Bullet update //
			// Update any bullets that are in-flight
			for (int i = 0; i < 100; i++)
			{
				if (bullets[i].isInFlight())
				{
					bullets[i].update(dtAsSeconds);
				}
			}
		}// End of updating SCENE

		// -== Scene Draw ==- //
		/*
		 **************
		 DRAW THE SCENE
		 **************
		 */

		if (state == State::PLAYING)
		{
			window.clear();
			// set the mainview to be displayed in the window
			// and draw everything related to it
			window.setView(mainView);
			// Draw the background
			window.draw(background, &textureBackground);

			// Draw the zombies
			for (int i = 0; i < numZombies; i++)
			{
				window.draw(zombies[i].getSprite());
			}
			// bullets draw //
			for (int i = 0; i <100; i++)
			{
				if (bullets[i].isInFlight())
				{
					window.draw(bullets[i].getShape());
				}
			}
			// Draw the player
			window.draw(player.getSprite());
		}
		if (state == State:: LEVELING_UP)
		{
		}
		if (state == State::PAUSED)
		{
		}
		if (state == State::GAME_OVER)
		{
		}
		window.display();

	}// End of the game loop
	delete[] zombies;
	return 0;
}