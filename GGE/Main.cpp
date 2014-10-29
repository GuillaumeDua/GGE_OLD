#include <iostream>
#include <iomanip>
#include "GGE.h"
#include "EventHandler.h"

using namespace GGE;

#include <SFML/Graphics.hpp>

int	main(int ac, char *av[])
{
	using namespace GGE;

	Game<GameEventHandler> game;

	// [Current_work] : Test
	

	
	Sprite sprite;
	Texture spriteTexture;
	Image SpriteImage;
	SpriteImage.create(32, 32, sf::Color::Green);
	spriteTexture.loadFromImage(SpriteImage);
	sprite.setTexture(spriteTexture);

	Entity * e = new Entity(sprite, Point<size_t>(200, 200));
	e->GetBehaviour().AddAction(Entity::State::READY, Behaviour<Entity>::Action([&](Entity & e) -> bool { e.MoveTo(Point<size_t>(400, 400)); return true; }));
	/*Behaviour<Entity>::Action cb = [&](Entity & e) -> bool { std::cout << "Entity : pos : " << e.GetPosition()._x << ',' << e.GetPosition()._y << std::endl; e.MoveTo(Point<size_t>(400, 400)); return true; };
	e->GetBehaviour().AddAction(Entity::State::READY, cb);*/

	game.AddEntities(e);
	// [Current_work] : End of test

	try
	{
		Texture background_texture;
		//std::cout << std::boolalpha << texture.loadFromFile("bg_Blue.png", sf::IntRect(0,0, 800, 600)) << std::endl;

		GGE::Image image;
		image.create(800, 600, sf::Color::Black);
		if (!(background_texture.loadFromImage(image)))
			throw std::exception("Cannot load background image");

		game.SetBackground(background_texture);
		// game.SetBackground("C:\\DEV\\PROJECTS\\GGE\\TEXTURES\\PNG\\bg_Blue.png");

		game.Start();
	}
	catch (std::exception & e)
	{
		std::cerr << e.what() << std::endl;
	}

	system("pause");
	return 0;
}
