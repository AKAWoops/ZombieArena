#include <sstream>
#include <SFML/Graphics.hpp>
#include "ZombieArena.h"
#include "Player.h"
#include "TextureHolder.h"
#include "Bullet.h"
#include "Pickup.h"

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

	// mouse pointer change
	// hide the mouse pointer and replace it with crosshair
	window.setMouseCursorVisible(true);
	Sprite spriteCrosshair;
	Texture textureCrosshair = TextureHolder::GetTexture("graphics/crosshair.png");
	spriteCrosshair.setTexture(textureCrosshair);
	spriteCrosshair.setOrigin(25, 25);
	//=== PICKUPS ===//
	// Create a couple of pickups
	Pickup healthPickup(1);
	Pickup ammoPickup(2);
	// About the game menu for score and stuff
	int score = 0;
	int hiScore = 0;

	/*
	 ***************************
	 *HEAD UP DISPLAY HUD STUFF*
	 ***************************
	 */

	// For the Home / Game over screen

	Sprite spriteGameOver;
	Texture textureGameOver = TextureHolder::GetTexture("graphics/background.png");
	spriteGameOver.setTexture(textureGameOver);
	spriteGameOver.setPosition(0, 0);
	// Create a view for the HUD
	View hudView(sf::FloatRect(0, 0, 1920, 1080));
	// Create a sprite for the ammo icon
	Sprite spriteAmmoIcon;
	Texture textureAmmoIcon = TextureHolder::GetTexture("graphics/ammo_icon.png");
	spriteAmmoIcon.setTexture(textureAmmoIcon);
	spriteAmmoIcon.setPosition(20, 980);
	// FONT //
	//Load the font
	Font font;
	font.loadFromFile("fonts/zombiecontrol.ttf");
	// Paused
	Text pausedText;
	pausedText.setFont(font);
	pausedText.setCharacterSize(155);
	pausedText.setFillColor(Color::White);
	pausedText.setPosition(400, 400);
	pausedText.setString("Press Enter \nto continue");
	// Game Over
	Text gameOverText;
	gameOverText.setFont(font);
	gameOverText.setCharacterSize(125);
	gameOverText.setFillColor(Color::White);
	gameOverText.setPosition(250, 850);
	gameOverText.setString("Press Enter To Play");
	// Leveling up screen
	Text levelUpText;
	levelUpText.setFont(font);
	levelUpText.setCharacterSize(80);
	levelUpText.setFillColor(Color::White);
	levelUpText.setPosition(150, 250);
	std::stringstream levelUpStream;
	levelUpStream <<
		"1 - Increased rate of fire" <<
		"\n2 - Increased clip size(next reload" <<
		"\n3 - Increased health" <<
		"\n4 - Increased run speed" <<
		"\n5 - More and better health pickups" <<
		"\n6 - More and better ammo pickups";
	levelUpText.setString(levelUpStream.str());
	// Ammo
	Text ammoText;
	ammoText.setFont(font);
	ammoText.setCharacterSize(55);
	ammoText.setFillColor(Color::White);
	ammoText.setPosition(200, 980);
	// Score
	Text scoretext;
	scoretext.setFont(font);
	scoretext.setCharacterSize(55);
	scoretext.setFillColor(Color::White);
	scoretext.setPosition(20, 0);
	// Hi Score
	Text hiScoreText;
	hiScoreText.setFont(font);
	hiScoreText.setCharacterSize(55);
	hiScoreText.setFillColor(Color::White);
	hiScoreText.setPosition(1400, 0);
	std::stringstream s;
	s << "Hi Score:" << hiScore;
	hiScoreText.setString((s.str()));
	// Zombies remaining
	Text zombiesRemainingText;
	zombiesRemainingText.setFont(font);
	zombiesRemainingText.setCharacterSize(55);
	zombiesRemainingText.setFillColor(Color::White);
	zombiesRemainingText.setPosition(1500, 980);
	zombiesRemainingText.setString("Zombies: 100");
	// Wave number
	int wave = 0;
	Text waveNumber;
	waveNumber.setFont(font);
	waveNumber.setCharacterSize(55);
	waveNumber.setFillColor(Color::White);
	waveNumber.setPosition(1250, 980);
	waveNumber.setString("Wave: 0");
	// Health Bar
	RectangleShape healthBar;
	healthBar.setFillColor(Color::Red);
	healthBar.setPosition(450, 980);
	// when did we last update the HUD?
	int framesSinceLastHUDUpdate = 0;
	// How often (in frames) should we update the HUD
	int fpsMeasurementFrameInterval = 1000;

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
				//=== PICKUPS ===//
				// configure the pick-ups
				healthPickup.setArena(arena);
				ammoPickup.setArena(arena);
				// create a horde of zombies //
				numZombies = 10;
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
			// === set mouse coords === //
			spriteCrosshair.setPosition(mouseWorldPosition);
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
			//=== PICKUPS ===//
			// update the pickups
			healthPickup.update(dtAsSeconds);
			ammoPickup.update(dtAsSeconds);
			/*
			***********************
			**COLLISION DETECTION**
			***********************
			*/
			//have any zombies been shot???
			for (int i = 0; i < 100; i++)
			{
				for (int j = 0; j < numZombies; j++)
				{
					if (bullets[i].isInFlight() &&
						zombies[j].isAlive())
					{
						if (bullets[i].getPosition().intersects
						(zombies[j].getPosition()))
						{
							// stop the bullet
							bullets[i].stop();
							// register the hit and see if it was a kill
							if (zombies[j].hit())
							{
								// not just a  hit but a kill too
								score += 10;
								if (score >= hiScore)
								{
									hiScore = score;
								}
								numZombiesAlive--;
								// when all the zombies are dead (again)
								if (numZombiesAlive == 0)
								{
									state = State::LEVELING_UP;
								}
							}
						}
					}
				}
			}// end zombies being shot

			/*
			 *********************
			 *COLLISION DETECTION*
			 *********************
			 */
			// have any zombies touched a player
			for (int i = 0; i < numZombies; i++)
			{
				if (player.getPosition().intersects
				(zombies[i].getPosition()) && zombies[i].isAlive())
				{
					if (player.hit(gameTimeTotal))
					{
						// add shit here later
					}
					if (player.getHealth() <= 0)
					{
						state = State::GAME_OVER;
					}
				}
			}// End player touched

			/*
			 *********************
			 *COLLISION DETECTION* ammo and health collision
			 *********************
			 */
			// has the player touched health pickup
			if (player.getPosition().intersects
			(healthPickup.getPosition()) && healthPickup.isSpawned())
			{
				player.increaseHealthLevel(healthPickup.gotIt());
			}
			// Has the player touched ammo pick up
			if (player.getPosition().intersects
			(ammoPickup.getPosition()) && ammoPickup.isSpawned())
			{
				bulletSpare += ammoPickup.gotIt();
			}

			/*
			 ******************
			 *HUD STUFF SIZING*
			 ******************
			 */
			// size up the health bar
			healthBar.setSize(Vector2f(player.getHealth() * 3, 50));
			// Increment the umber of frames since the previous update
			framesSinceLastHUDUpdate++;
			// recalculate every fpsMeasurementFrameInterval frames
			if (framesSinceLastHUDUpdate > fpsMeasurementFrameInterval)
			{
				/*
				 ************
				 *Update HUD*
				 ************
				 */
				// Update game HUD text
				std::stringstream ssAmmo;
				std::stringstream ssScore;
				std::stringstream ssHiScore;
				std::stringstream ssWave;
				std::stringstream ssZombiesAlive;
				// Update the ammo text
				ssAmmo << bulletsInClip << "/" << bulletSpare;
				ammoText.setString(ssAmmo.str());
				// Update the score test
				ssScore << "Score:" << score;
				scoretext.setString(ssScore.str());
				// Update the high score text
				ssHiScore << "Hi Score:" << hiScore;
				hiScoreText.setString((ssHiScore.str()));
				//Update the wave
				ssWave << "Wave:" << wave;
				waveNumber.setString(ssWave.str());
				// Update the high score test
				ssZombiesAlive << "Zombies:" << numZombiesAlive;
				zombiesRemainingText.setString(ssZombiesAlive.str());
				framesSinceLastHUDUpdate = 0;
			}//End HUD Update

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
			// HUD STUFF //
			//Switch to the HUD view
			window.setView(hudView);
			// Draw all the HUD elements
			window.draw(spriteAmmoIcon);
			window.draw(ammoText);
			window.draw(scoretext);
			window.draw(hiScoreText);
			window.draw(healthBar);
			window.draw(waveNumber);
			window.draw(zombiesRemainingText);
			//=== PICKUPS ===//
			// draw the pickups if currently spawned
			if (ammoPickup.isSpawned())
			{
				window.draw(ammoPickup.getSprite());
			}
			if (healthPickup.isSpawned())
			{
				window.draw(healthPickup.getSprite());
			}
			// draw the crosshair
			window.draw(spriteCrosshair);
		}
		if (state == State:: LEVELING_UP)
		{
			window.draw(spriteGameOver);
			window.draw(levelUpText);
		}
		if (state == State::PAUSED)
		{
			window.draw(pausedText);
		}
		if (state == State::GAME_OVER)
		{
			window.draw(spriteGameOver);
			window.draw(gameOverText);
			window.draw(scoretext);
			window.draw(hiScoreText);
		}
		window.display();

	}// End of the game loop
	delete[] zombies;
	return 0;
}