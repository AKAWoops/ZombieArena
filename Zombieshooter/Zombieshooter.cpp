#include "Zombieshooter.h"

#include <SFML/Graphics.hpp>
#include "Player.h"

using namespace sf;
//test tes test test test
//test tes test test test
//test tes test test test
//test tes test test test
int main()
{
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
	Texture textureBackground;
	textureBackground.loadFromFile("graphics/background_sheet.png");

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
		}// End WASD while playing

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
	return 0;
}