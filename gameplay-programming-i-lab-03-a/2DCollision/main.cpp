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
	sf::Texture capsule_texture;
	if (!capsule_texture.loadFromFile("assets\\capsule.jpg")) {
		DEBUG_MSG("Failed to load file");
		return EXIT_FAILURE;
	}
	sf::Texture ray_texture;
	if (!ray_texture.loadFromFile("assets\\ray.png")) {
		DEBUG_MSG("Failed to load file");
		return EXIT_FAILURE;
	}
	sf::Texture polygon_texture;
	if (!polygon_texture.loadFromFile("assets\\polygon.png")) {
		DEBUG_MSG("Failed to load file");
		return EXIT_FAILURE;
	}
	// Setup a mouse Sprite
	sf::Sprite rayForCollision;
	rayForCollision.setTexture(ray_texture);
	rayForCollision.setPosition(20, 400);

	sf::Sprite mouse;
	mouse.setTexture(mouse_texture);

	sf::CircleShape mouseCircle;
	mouseCircle.setRadius(40);
	mouseCircle.setFillColor(sf::Color::Red);
	mouseCircle.setOrigin(30, 30);

	sf::Sprite capsuleForCollision;
	capsuleForCollision.setTexture(capsule_texture);
	capsuleForCollision.setPosition(300, 50);

	sf::Sprite polygonForCollision;
	polygonForCollision.setTexture(polygon_texture);
	polygonForCollision.setPosition(400, 400);

	sf::CircleShape circleForCollision;
	circleForCollision.setRadius(50);
	circleForCollision.setFillColor(sf::Color::Blue);
	circleForCollision.setPosition(700, 300);
	


	//movement of user aabb/circle/ray
	//Setup mouse AABB
	c2AABB aabb_mouse;
	aabb_mouse.min = c2V(mouse.getPosition().x, mouse.getPosition().y);
	aabb_mouse.max = c2V(mouse.getGlobalBounds().width, mouse.getGlobalBounds().height);

	//mouse circle colisions
	c2Circle circle_mouse;
	circle_mouse.p = c2V(mouseCircle.getPosition().x, mouseCircle.getPosition().y);
	circle_mouse.r = mouseCircle.getRadius();

	c2Ray ray_mouse;
	
	// Setup Players Default Animated Sprite
	AnimatedSprite animated_sprite(sprite_sheet);
	animated_sprite.addFrame(sf::IntRect(3, 3, 84, 84));
	animated_sprite.addFrame(sf::IntRect(88, 3, 84, 84));
	animated_sprite.addFrame(sf::IntRect(173, 3, 84, 84));
	animated_sprite.addFrame(sf::IntRect(258, 3, 84, 84));
	animated_sprite.addFrame(sf::IntRect(343, 3, 84, 84));
	animated_sprite.addFrame(sf::IntRect(428, 3, 84, 84));

	// Setup Players AABB(in the corner)
	// Setup Players AABB
	c2AABB aabb_player;
	aabb_player.min = c2V(animated_sprite.getPosition().x, animated_sprite.getPosition().y);
	aabb_player.max = c2V(animated_sprite.getGlobalBounds().width / animated_sprite.getFrames().size(),
		animated_sprite.getGlobalBounds().height / animated_sprite.getFrames().size());


	c2Capsule capsule_player;
	capsule_player.a = c2V(capsuleForCollision.getPosition().x + capsuleForCollision.getGlobalBounds().height / 2, capsuleForCollision.getPosition().y + capsuleForCollision.getGlobalBounds().height / 2);
	capsule_player.b = c2V(capsuleForCollision.getPosition().x + capsuleForCollision.getGlobalBounds().width - capsuleForCollision.getGlobalBounds().height / 2, capsuleForCollision.getPosition().y + capsuleForCollision.getGlobalBounds().height / 2);
	capsule_player.r = capsuleForCollision.getGlobalBounds().height / 2;



	c2Ray ray_player;
	ray_player.p = c2V(20, 400);
	ray_player.d = c2Norm(c2V(0, 1));
	ray_player.t = 50;

	c2Circle circle_player;
	circle_player.p = c2V(circleForCollision.getPosition().x, circleForCollision.getPosition().y);
	circle_player.r = circleForCollision.getRadius();

	//updat
	c2Poly poly_player;
	poly_player.count = 4;
	poly_player.verts[0] = c2V(polygonForCollision.getPosition().x, polygonForCollision.getPosition().y);
	poly_player.verts[1] = c2V(polygonForCollision.getPosition().x + 100, polygonForCollision.getPosition().y);
	poly_player.verts[2] = c2V(polygonForCollision.getPosition().x, polygonForCollision.getPosition().y + 100);
	poly_player.verts[3] = c2V(polygonForCollision.getPosition().x + 100, polygonForCollision.getPosition().y + 100);



	// Setup the Player
	Player player(animated_sprite);
	Input input;

	// Collision result
	int result = 0;

	//switch between aabb/circle/ray
	int shapeNumber=1;
	// Start the game loop
	while (window.isOpen())
	{
		// Move Sprite Follow Mouse(rectangle)
		mouse.setPosition(window.mapPixelToCoords(sf::Mouse::getPosition(window)));

		// Move Sprite Follow Mouse(circle)
		mouseCircle.setPosition(window.mapPixelToCoords(sf::Mouse::getPosition(window)));

		// Update mouse AABB
		aabb_mouse.min = c2V(mouse.getPosition().x, mouse.getPosition().y);
		aabb_mouse.max = c2V(mouse.getPosition().x + mouse.getGlobalBounds().width, mouse.getPosition().y + mouse.getGlobalBounds().width);

		//update mouse circle
		circle_mouse.p = c2V(mouseCircle.getPosition().x, mouseCircle.getPosition().y);
	
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
				//shape is AABB
				else if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
				{
					shapeNumber = 1;
				}
				//shape is a circle
				else if (sf::Keyboard::isKeyPressed(sf::Keyboard::C))
				{
					shapeNumber = 2;
				}
				//shape is a ray.
				else if (sf::Keyboard::isKeyPressed(sf::Keyboard::R))
				{
					shapeNumber = 3;
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

		// Check for collisions 1=AABB 2=CIRCLE 3=RAY
		//AABB
		if (shapeNumber == 1)
		{	
			//AABB TO AABB
			result = c2AABBtoAABB(aabb_mouse, aabb_player);
			cout << ((result != 0) ? ("Collision AABB TO AABB") : "") << endl;
			if (result) {
				player.getAnimatedSprite().setColor(sf::Color(255, 0, 0));
			}
			else {
				player.getAnimatedSprite().setColor(sf::Color(0, 255, 0));
			}
			//AABB TO CAPSULE
			result = c2AABBtoCapsule(aabb_mouse, capsule_player);
			if(result)
			{
				cout << ((result != 0) ? ("Collision AABB TO CAPSULE") : "") << endl;
			}
			//AABB TO RAY
			c2Raycast cast;
			result = c2RaytoAABB(ray_player, aabb_mouse, &cast);
			if (result)
			{
				cout << ((result != 0) ? ("Collision AABB TO RAY") : "") << endl;
			}
			//AABB TO POLY
			result = c2AABBtoPoly(aabb_mouse, &poly_player, NULL);
			if (result)
			{
				cout << ((result != 0) ? ("Collision AABB TO POLY") : "") << endl;
			}
		}
		//CIRCLE
		if(shapeNumber==2)
		{
			result = c2CircletoAABB(circle_mouse, aabb_player);
			if (result) {
				cout << ((result != 0) ? ("Collision CIRCLE TO AABB") : "") << endl;
			}

			result = c2CircletoCircle(circle_player, circle_mouse);
			if (result) {
				cout << ((result != 0) ? ("Collision CIRCLE TO CIRCLE") : "") << endl;
			}
			c2Raycast cast;
			result = c2RaytoCircle(ray_player, circle_mouse, &cast);
			if (result)
			{
				cout << ((result != 0) ? ("Collision CIRCLE TO RAY") : "") << endl;
			}
			result = c2CircletoCapsule(circle_mouse, capsule_player);
			if (result)
			{
				cout << ((result != 0) ? ("Collision CIRCLE TO CAPSULE") : "") << endl;
			}
			result = c2CircletoPoly(circle_mouse, &poly_player, NULL);
			if (result)
			{
				cout << ((result != 0) ? ("Collision CIRCLE TO POLY") : "") << endl;
			}
			
			
		}
	
	


		//cout << ((result != 0) ? ("Collision") : "") << endl;
		//if (result){
		//	player.getAnimatedSprite().setColor(sf::Color(255,0,0));
		//}
		//else {
		//	player.getAnimatedSprite().setColor(sf::Color(0, 255, 0));
		//}

		// Clear screen
		window.clear();

		// Draw the Players Current Animated Sprite
		window.draw(player.getAnimatedSprite());
		if(shapeNumber==1)
		{
			window.draw(mouse);
		}
		if (shapeNumber == 2)
		{
			window.draw(mouseCircle);
		}
		
		window.draw(circleForCollision);
		window.draw(capsuleForCollision);
		window.draw(rayForCollision);
		window.draw(polygonForCollision);

		// Update the window
		window.display();
	}

	return EXIT_SUCCESS;
};