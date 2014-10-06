#include <iostream>

#include "GGE.h"

using namespace GGE;

#include <SFML/Graphics.hpp>

int	main(int ac, char *av[])
{
	GGE::Game				game;
	GGE::GameEventHandler	eventsHandler(game);

	game.SetGameEventsHandler(eventsHandler);
	game.Start();

	system("pause");
	return 0;
}
