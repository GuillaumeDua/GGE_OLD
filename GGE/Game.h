#ifndef __GEE_GAME__
# define __GEE_GAME__

# include <SFML\Window\Window.hpp>
# include <SFML\Graphics\RenderWindow.hpp>
# include <iostream>
# include <atomic>
# include "IGameEventHandler.h"
# include "EventHandler.h"

// Window::SetFramerateLimit => vertical sync ?
// screenshots => sf::Image Scren = App.Capture()

namespace GGE
{
	class Game
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

		// Run
		bool	Start(void)
		{
			assert(this->_EventHandler);

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
			this->_window.clear();
			this->_window.display();
		}

		// Events handling
		bool	HandleEvent(const sf::Event & event)
		{
			this->_EventHandler->Realize(event);
			return true;
			switch (event.type)
			{
			case sf::Event::Closed:
				this->_IsRunning = false; // Notifiy stop
				return false;
			case sf::Event::Resized:
				std::cout << "Resized : " << event.size.width << 'x' << event.size.height << std::endl;
				break;
			case sf::Event::LostFocus:
				std::cout << "Focus lost" << std::endl;
				break;
			case sf::Event::GainedFocus:
				std::cout << "Focus gained" << std::endl;
				break;
			case sf::Event::KeyPressed:
				std::cout << "KeyPress : " << event.key.code << std::endl;
				break;
			case sf::Event::KeyReleased:
				std::cout << "KeyReleased : " << event.key.code << std::endl;
				break;
			case sf::Event::TextEntered:
				std::cout << "Text entered : [" << (event.text.unicode < 128 ? static_cast<char>(event.text.unicode) : static_cast<unsigned int>(event.text.unicode)) << ']' << std::endl;
				break;
			case sf::Event::MouseWheelMoved:
				std::cout << "Wheel movement : " << event.mouseWheel.delta << " on : " << event.mouseWheel.x << 'x' << event.mouseWheel.y << std::endl;
				break;
			case sf::Event::MouseButtonPressed:
				std::cout << "MouseButtonPressed : " << event.mouseButton.x << 'x' << event.mouseButton.y << std::endl;
				break;
			case sf::Event::MouseMoved:
				std::cout << "MouveMoved : " << event.mouseMove.x << 'x' << event.mouseMove.y << std::endl;
				break;
			case sf::Event::MouseEntered:
				std::cout << "MouseEntered" << std::endl;
				break;
			case sf::Event::MouseLeft:
				std::cout << "MouseLeft" << std::endl;
				break;
				// [todo] : JoyStick
			default:
				break;
			}
		}
		void	ManageEvents(void)
		{
			sf::Event event;

			// todo : associate a callback to each of that event.type
			while (this->_window.pollEvent(event))
			{
				std::cout << "New event : " << event.type << std::endl;
				if (this->HandleEvent(event) == false) break;
			}
		}

		// Loop
		bool	Update(void)
		{
			this->ManageEvents();
			this->Render();
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

		// Event handling :
				using CB = std::function<bool(const sf::Event & event, Game & game)>;
				GGE::IGameEventHandler<CB>	* _EventHandler;// = new GameEventHandler(*this); // Binded by default with the current instance
	};
}

#endif // __GEE_GAME__