#ifndef __GGE_IGAMEEVENTHANDLER__
# define __GGE_IGAMEEVENTHANDLER__

# include "SFML\Window\Event.hpp"

template <typename CB>
struct IGameEventHandler
{
	CB &	Get(const sf::Event::EventType & eventType) = 0;
	bool	Realize(const sf::Event & event)			= 0;
};

#endif // __GGE_IGAMEEVENTHANDLER__