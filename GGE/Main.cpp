#include <iostream>
#include "GGE.h"
#include "EventHandler.h"

using namespace GGE;

#include <SFML/Graphics.hpp>

int	main(int ac, char *av[])
{
	using namespace GGE;

	Game<GameEventHandler> game;

	game.Start();

	system("pause");
	return 0;
}
