#ifndef __GGE_EVENT_HANDLER__
# define __GGE_EVENT_HANDLER__

# include <SFML\Window\Event.hpp>
# include "Notification.h"

// # include "Game.h"
# include "IGameEventHandler.h"

# include <map>
# include <functional>
# include <exception>
# include <stdexcept>

// encapsulate std::function into an object that handle task's priority ?
// encapsulate std::function into an object that handle threads ?
// Use std::bind + placeholder

namespace GGE
{
	class Game;

	using CB = std::function<bool(const sf::Event & event, Game & game)>;		// CB can be a callback, a closure, etc ...
	//typedef std::function< bool(const sf::Event & event, Game & game) >	CB;		// CB can be a callback, a closure, etc ...

	struct GameEventHandler : public IGameEventHandler<CB>
	{
		GameEventHandler(Game & game)
			: _game(game)
		{}
		GameEventHandler(const GameEventHandler &) = delete;
		GameEventHandler(const GameEventHandler &&) = delete;
		GameEventHandler()  = delete;
		~GameEventHandler(){}

		CB &	Get(const sf::Event::EventType & eventType) // throw std::out_of_range
		{
			return this->_eventTypeToCB_map.at(eventType);
		}
		bool	Realize(const sf::Event & event)
		{
			try
			{
				CB & cb = this->_eventTypeToCB_map.at(event.type); // can trigger std::out_of_range
				return cb(event, this->_game);
			}
			catch (const std::out_of_range &)
			{
				std::cerr << "GGE::GameEventHandler::Realize : Out-of-range exception triggered" << std::endl;
			}
			return false;
		}

	protected:
		Game & _game;	// binded
		// NotificationHolder &	_notificationHolder;

		std::map<const sf::Event::EventType, CB>	_eventTypeToCB_map/* = 
		{
			{
				sf::Event::Closed,
				[](const sf::Event & event, Game & game){ std::cout << "Windows close event called" << std::endl; game._window.close(); }
			},
			{
				sf::Event::Resized,
				[](const sf::Event & event, Game & game){ std::cout << "Resized : " << event.size.width << 'x' << event.size.height << std::endl; }
			}
		}*/;
	};
}

#endif // __GGE_EVENT_HANDLER__