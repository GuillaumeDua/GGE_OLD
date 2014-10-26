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
	Entity * e = new Entity(Sprite(), Point<size_t>(200, 200));
	Behaviour<Entity>::Action cb = [&](Entity & e) -> bool { e.MoveTo(Point<size_t>(400,400)); };
	
	e->GetBehaviour().AddAction(Entity::State::READY, cb);
	game.AddEntities(e);

	// [Current_work] : End of test
	game.Start();

	system("pause");
	return 0;
}
