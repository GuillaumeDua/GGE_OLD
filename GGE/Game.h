#ifndef __GEE_GAME__
# define __GEE_GAME__

# include <SFML\Window\Window.hpp>
# include <SFML\Graphics\RenderWindow.hpp>
# include <iostream>
# include <atomic>
# include <functional>
# include <map>

# include "Entity.h"
# include "GCL/Exception.h"

# include "EventHandler.h"

// Window::SetFramerateLimit => vertical sync ?
// screenshots => sf::Image Scren = App.Capture()


namespace GGE
{
	

	struct EmptyEventHolder {}; // Error -> Invalid event-holder
	// template <typename EventHolder = EmptyEventHolder> class Game
	class Game
	{
	public:
		Game()
			: _window(sf::VideoMode(800, 600, 32), "GEE Rendering")
		{
			this->Initialize();
		}
		Game(const size_t TicksPerSec)
			: _TicksPerSec(TicksPerSec)
			, _window(sf::VideoMode(800, 600, 32), "GEE Rendering")
		{
			this->Initialize();
		}
		Game(const Game &)	= delete;
		Game(const Game &&) = delete;
		~Game(){}

		void							Initialize(void)
		{
			// _window.setFramerateLimit
			// _window.getView
			if (!(_window.isOpen()))
				throw GCL::Exception("[Error] : Rendering window is not open");
		}

		// Run [-> I cld use my own Runnable class]
		bool							Start(void)
		{
			if (this->_IsRunning) return false;
			this->_IsRunning = true;
			std::cout << "[+] Start ... framerate set at : " << FPS << std::endl;
			bool ret = this->Loop();
			this->_window.close();
			return ret;
		}
		bool							Stop(void)
		{
			this->_IsRunning = false;
		}

		// Rendering
		inline void						SetBackground(const Sprite & sprite)
		{
			this->_backgroundSprite = sprite;
		}
		inline void						SetBackground(const std::string & texture_path)
		{
			if (!(_bufBatckgroundTexture.loadFromFile(texture_path)))
				throw GCL::Exception("[Error] : GGE::Game::SetBackground : Cannot load texture from file : " + texture_path);
			this->_backgroundSprite.setTexture(_bufBatckgroundTexture);
		}
		inline void						SetBackground(const Texture & texture)
		{
			this->_backgroundSprite.setTexture(texture);
		}

		// Events handling
		inline void						SetEventHandler(EventHandler::Interface & eventHandlerInterface)
		{
			this->_EventTypeToCB = eventHandlerInterface.GetTypeToCB_Map();
		}
		bool							HandleEvent(const sf::Event & event)
		{
			EventHandler::Interface::MapType::const_iterator it = this->_EventTypeToCB.find(event.type);
			if (it != this->_EventTypeToCB.end())
				it->second(event, *this);
			return true;
		}
		void							ManageEvents(void)
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
		void							AddEntities(Entity * entity)
		{
			this->_entities.push_back(entity);
		}
		void							ManageEntities()
		{
			for (std::vector<Entity*>::iterator it = this->_entities.begin(); it != this->_entities.end(); ++it)
			{
				(*it)->Behave();
				(*it)->Draw(this->_window);
			}
		}

		// Loop
		bool							Update(void)
		{
			this->_window.clear();
			this->_window.draw(this->_backgroundSprite);
			this->ManageEvents();
			this->ManageEntities();
			this->_window.display();

			return true;
		}
		bool							Loop(void)
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

		// Ticks :
		const float						FPS = 60.f;
		const sf::Time 					TimePerFrame = sf::seconds(1.f / FPS);

		static	const size_t			DEFAULT_TICKS_PER_SEC	= 50;
		static	const size_t			DEFAULT_FRAME_SKIP		= 10;
				const size_t			_TicksPerSec			= DEFAULT_TICKS_PER_SEC;
				const size_t			_TicksToSkip			= 1000 / _TicksPerSec;
				const size_t			_MaxFameSkip			= DEFAULT_FRAME_SKIP;
				std::atomic<bool>		_IsRunning				= false;

		// Rendering :
				// [Todo] : Use GGE::Screen here
				RenderWindow			_window;
				Sprite					_backgroundSprite;
				Texture					_bufBatckgroundTexture; // To use as buffer. [Todo]=[To_test] -> SetSmooth

		// Entities :
				std::vector<Entity*>	_entities;

		// EventsHandler :
				EventHandler::Interface::MapType &	_EventTypeToCB = EventHandler::Debugger().GetTypeToCB_Map();
	};
}

#endif // __GEE_GAME__