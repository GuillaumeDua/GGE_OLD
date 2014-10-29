#include <iostream>
#include "GGE.h"
#include "EventHandler.h"

using namespace GGE;

#include <SFML/Graphics.hpp>

int	main(int ac, char *av[])
{
	using namespace GGE;

	Game<GameEventHandler> game;

	// [Current_work] : Test
	

	/*
	Sprite sprite;
	sprite.setColor(sf::Color::Blue);

	Entity * e = new Entity(sprite, Point<size_t>(200, 200));
	Behaviour<Entity>::Action cb = [&](Entity & e) -> bool { e.MoveTo(Point<size_t>(400, 400)); return true; };
	e->GetBehaviour().AddAction(Entity::State::READY, cb);

	game.AddEntities(e);*/
	// [Current_work] : End of test

	try
	{
		game.SetBackground("C:\\DEV\\PERSO\\GGE\\TEXTURES\\BMP\\bg_BlueWood.bmp");
		game.Start();
	}
	catch (std::exception & e)
	{
		std::cerr << e.what() << std::endl;
	}

	system("pause");
	return 0;
}
