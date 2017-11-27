#include <iostream>
#include <SFML/Graphics.hpp>
#define TINYC2_IMPL
#include <tinyc2.h>
#include <AnimatedSprite.h>
#include <Player.h>
#include <Input.h>
#include <Debug.h>

using namespace std;

int main()
{
	// Create the main window
	sf::RenderWindow window(sf::VideoMode(800, 600), "SFML window");

	// Load a sprite to display
	sf::Texture sprite_sheet;
	if (!sprite_sheet.loadFromFile("assets\\grid.png")) {
		DEBUG_MSG("Failed to load file");
		return EXIT_FAILURE;
	}

	// Load a mouse texture to display
	sf::Texture mouse_texture;
	if (!mouse_texture.loadFromFile("assets\\mouse.png")) {
		DEBUG_MSG("Failed to load file");
		return EXIT_FAILURE;
	}

	// Setup a mouse Sprite
	sf::Sprite mouse;
	mouse.setTexture(mouse_texture);

	//Setup mouse AABB
	c2AABB aabb_mouse;
	//aabb_mouse.min = c2V(mouse.getPosition().x, mouse.getPosition().y);
	//aabb_mouse.max = c2V(mouse.getGlobalBounds().width, mouse.getGlobalBounds().width);

	//Setup mouse capsule
	c2Capsule capsule_mouse;
	capsule_mouse.a = c2V(mouse.getPosition().x, mouse.getPosition().y);
	capsule_mouse.b = c2V(mouse.getGlobalBounds().width, mouse.getGlobalBounds().width); 
	capsule_mouse.r =mouse.getGlobalBounds().height/2;

	//Setup mouse polygon
	c2Poly polygon_mouse;
	//polygon_mouse.count = 3;
	//polygon_mouse.verts[0] = c2V(mouse.getPosition().x, mouse.getPosition().y);
	//polygon_mouse.verts[1] = c2V(mouse.getPosition().x+70, mouse.getPosition().y+70);
	//polygon_mouse.verts[2] = c2V(mouse.getGlobalBounds().width, mouse.getGlobalBounds().width);
	//c2MakePoly(&polygon_mouse);

	//Setup mouse ray
	c2Ray ray_mouse;
	//ray_mouse.d=c2Norm(c2V(mouse.getGlobalBounds().width,mouse.getGlobalBounds().width));
	//ray_mouse.p = c2V(mouse.getPosition().x, mouse.getPosition().y);
	//ray_mouse.t=1;

	//Setup mouse circlee
	c2Circle circle_mouse;
	//circle_mouse.p = c2V(mouse.getPosition().x, mouse.getPosition().y); 
	//circle_mouse.r = mouse.getGlobalBounds().height / 2;

	// Setup Players Default Animated Sprite
	AnimatedSprite animated_sprite(sprite_sheet);
	animated_sprite.addFrame(sf::IntRect(3, 3, 84, 84));
	animated_sprite.addFrame(sf::IntRect(88, 3, 84, 84));
	animated_sprite.addFrame(sf::IntRect(173, 3, 84, 84));
	animated_sprite.addFrame(sf::IntRect(258, 3, 84, 84));
	animated_sprite.addFrame(sf::IntRect(343, 3, 84, 84));
	animated_sprite.addFrame(sf::IntRect(428, 3, 84, 84));

	// Setup Players AABB
	c2AABB aabb_player;
	aabb_player.min = c2V(animated_sprite.getPosition().x, animated_sprite.getPosition().y);
	aabb_player.max = c2V(animated_sprite.getGlobalBounds().width / animated_sprite.getFrames().size(), 
		animated_sprite.getGlobalBounds().height / animated_sprite.getFrames().size());


	// Setup the Player
	Player player(animated_sprite);
	Input input;

	// Collision result
	int result = 0;
	
	// Start the game loop
	while (window.isOpen())
	{
		// Move Sprite Follow Mouse
		mouse.setPosition(window.mapPixelToCoords(sf::Mouse::getPosition(window)));

		// Update mouse AABB
		//aabb_mouse.min = c2V(mouse.getPosition().x, mouse.getPosition().y);
		//aabb_mouse.max = c2V(mouse.getGlobalBounds().width, mouse.getGlobalBounds().width);
		//Update mouse capsule
	/*	capsule_mouse.a = c2V(mouse.getPosition().x, mouse.getPosition().y);
		capsule_mouse.b = c2V(mouse.getGlobalBounds().width, mouse.getGlobalBounds().width);
		capsule_mouse.r = mouse.getGlobalBounds().height / 2;
*/

		//Update mouse polygon
		/*polygon_mouse.verts[0] = c2V(mouse.getPosition().x, mouse.getPosition().y);
		polygon_mouse.verts[1]= c2V(mouse.getPosition().x + 1, mouse.getPosition().y + 1);
		polygon_mouse.verts[2] = c2V(mouse.getGlobalBounds().width, mouse.getGlobalBounds().width);*/

		//Update mouse ray
		/*ray_mouse.d = c2Norm(c2V(mouse.getGlobalBounds().width, mouse.getGlobalBounds().width));
		ray_mouse.p = c2V(mouse.getPosition().x, mouse.getPosition().y);*/

		//Update mouse circle
		//circle_mouse.p = c2V(mouse.getPosition().x, mouse.getPosition().y);
		//circle_mouse.r = mouse.getGlobalBounds().height / 2;

		// Process events
		sf::Event event;
		while (window.pollEvent(event))
		{
			switch (event.type)
			{
			case sf::Event::Closed:
				// Close window : exit
				window.close();
				break;
			case sf::Event::KeyPressed:
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
				{
					input.setCurrent(Input::Action::LEFT);
				}
				else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
				{
					input.setCurrent(Input::Action::RIGHT);
				}
				else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
				{
					input.setCurrent(Input::Action::UP);
				}
				break;
			default:
				input.setCurrent(Input::Action::IDLE);
				break;
			}
		}

		// Handle input to Player
		player.handleInput(input);

		// Update the Player
		player.update();
	/*	c2Raycast cast;*/
		// Check for collisions
		//result = c2AABBtoAABB(aabb_mouse, aabb_player);
		result = c2AABBtoCapsule(aabb_player, capsule_mouse);
 		//result = c2AABBtoPoly(aabb_player, &polygon_mouse, NULL);
		/*result = c2RaytoAABB(ray_mouse, aabb_player, &cast);*/
		//result = c2CircletoAABB(circle_mouse, aabb_player);
		cout << ((result != 0) ? ("Collision") : "") << endl;
		if (result){
			player.getAnimatedSprite().setColor(sf::Color(255,0,0));
		}
		else {
			player.getAnimatedSprite().setColor(sf::Color(0, 255, 0));
		}

		// Clear screen
		window.clear();

		// Draw the Players Current Animated Sprite
		window.draw(player.getAnimatedSprite());
		window.draw(mouse);

		// Update the window
		window.display();
	}

	return EXIT_SUCCESS;
};