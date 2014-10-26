#ifndef __GEE_GAME__
# define __GEE_GAME__

# include <SFML\Window\Window.hpp>
# include <SFML\Graphics\RenderWindow.hpp>
# include <iostream>
# include <atomic>
# include <functional>
# include <map>
# include "Entity.h"

// Window::SetFramerateLimit => vertical sync ?
// screenshots => sf::Image Scren = App.Capture()

namespace GGE
{
	struct EmptyEventHolder {}; // Error -> Invalid event-holder
	template <typename EventHolder = EmptyEventHolder> class Game
	{
	public:
		Game()
			: _window(sf::VideoMode(800, 600, 32), "GEE Rendering")
		{}
		Game(const size_t TicksPerSec)
			: _TicksPerSec(TicksPerSec)
			, _window(sf::VideoMode(800, 600, 32), "GEE Rendering")
		{}
		Game(const Game &)	= delete;
		Game(const Game &&) = delete;
		~Game(){}

//protected:

		// Run [-> I cld use my own Runnable class]
		bool	Start(void)
		{
			if (this->_IsRunning) return false;
			this->_IsRunning = true;
			std::cout << "[+] Start ... framerate set at : " << FPS << std::endl;
			bool ret = this->Loop();
			this->_window.close();
			return ret;
		}
		bool	Stop(void)
		{
			this->_IsRunning = false;
		}

		// Rendering
		void	Render(void)
		{
			this->_window.display();
		}

		// Events handling
		bool	HandleEvent(const sf::Event & event)
		{
			EventHolder::MapType::const_iterator it = EventHolder::_eventTypeToCB_map.find(event.type);
			if (it != EventHolder::_eventTypeToCB_map.end())
				it->second(event, *this);
			return true;
		}
		void	ManageEvents(void)
		{
			sf::Event event;

			// todo : associate a callback to each of that event.type
			while (this->_window.pollEvent(event))
			{
				// std::cout << "New event : " << event.type << std::endl;
				if (this->HandleEvent(event) == false) break;
			}
		}

		// Entities managing
		void	AddEntities(Entity * entity)
		{
			this->_entities.push_back(entity);
		}
		void	ManageEntities()
		{
			for (std::vector<Entity*>::iterator it = this->_entities.begin(); it != this->_entities.end(); ++it)
			{
				(*it)->Behave();
				(*it)->Draw();
			}
		}

		// Loop
		bool	Update(void)
		{
			this->_window.clear();
			this->ManageEvents();
			this->ManageEntities();
			this->Render();

			return true;
		}
		bool	Loop(void)
		{
			sf::Clock	clock;
			sf::Time	diffTimeCt = sf::Time::Zero;

			while (this->_IsRunning)
			{
				sf::Time elaspedTime = clock.restart();
				diffTimeCt += elaspedTime;

				while (this->_IsRunning && diffTimeCt > TimePerFrame)
				{
					diffTimeCt -= TimePerFrame;
					if (this->Update() == false) return false;
				}
			}
			return true;
		}

		const float					FPS = 60.f;
		const sf::Time 				TimePerFrame = sf::seconds(1.f / FPS);

		static	const size_t		DEFAULT_TICKS_PER_SEC	= 50;
		static	const size_t		DEFAULT_FRAME_SKIP		= 10;
				const size_t		_TicksPerSec			= DEFAULT_TICKS_PER_SEC;
				const size_t		_TicksToSkip			= 1000 / _TicksPerSec;
				const size_t		_MaxFameSkip			= DEFAULT_FRAME_SKIP;
				std::atomic<bool>	_IsRunning				= false;

		// Rendering :
				sf::RenderWindow	_window;

		// Entities :
				std::vector<Entity*>	_entities;
	};
}

#endif // __GEE_GAME__