#ifndef __GGE_IGAMEEVENTHANDLER__
# define __GGE_IGAMEEVENTHANDLER__

# include "SFML\Window\Event.hpp"

namespace GGE
{
	template <typename CB>
	struct IGameEventHandler
	{
		virtual CB &	Get(const sf::Event::EventType & eventType) = 0;
		virtual bool	Realize(const sf::Event & event)			= 0;
	};
}
#endif // __GGE_IGAMEEVENTHANDLER__