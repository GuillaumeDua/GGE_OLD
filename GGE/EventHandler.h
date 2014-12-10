#ifndef __GGE_EVENT_HANDLER__
# define __GGE_EVENT_HANDLER__

# include <SFML\Window\Event.hpp>
# include "Notification.h"

# include <iostream>
# include <map>
# include <functional>
# include <exception>
# include <stdexcept>

// # include "Game.h"

///////////////////////////////////////
//
// encapsulate std::function into an object that handle task's priority ?
// encapsulate std::function into an object that handle threads ?
// Use std::bind + placeholder
//
// [Todo] : std::map[EventType] -> std::vector<CB>
//			Plusieurs CB binded au meme event
//
///////////////////////////////////////

namespace GGE
{
	/*
	[Todo] : Keyboard event -> Do a stack of inputs
	[Todo] : Mouse event
	*/
	class Game;

	namespace EventHandler
	{
		using GameType = Game;
		using CB = std::function<bool(const sf::Event & event, GameType & game)>;			// CB can be a callback, a closure, obj-binded function members with place-holders etc ...
		using MapType = std::map < const sf::Event::EventType, CB >;

		struct Engine
		{
			static MapType & GetTypeToCB_Map(void)
			{
				std::cout << "[+] Engine::GetTypeToCB_Map" << std::endl;
				return _eventTypeToCB_map;
			}

			static MapType	_eventTypeToCB_map;
		};
		struct Debugger
		{
			static MapType & GetTypeToCB_Map(void)
			{
				std::cout << "[+] Debugger::GetTypeToCB_Map" << std::endl;
				return _eventTypeToCB_map;
			}

			static MapType	_eventTypeToCB_map;
		};
		struct Editor
		{
			static MapType & GetTypeToCB_Map(void)
			{
				std::cout << "[+] Editor::GetTypeToCB_Map" << std::endl;
				return _eventTypeToCB_map;
			}

			static MapType	_eventTypeToCB_map;
		};
	}
}

#endif // __GGE_EVENT_HANDLER__