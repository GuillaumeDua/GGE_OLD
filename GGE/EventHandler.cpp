#include "EventHandler.h"
#include "Rendering.h"

# include "Game.h"

///////////////////////////////////////
//
// Sample of implementation
//
///////////////////////////////////////

namespace TEST
{
	GGE::Sprite sprite;
	GGE::Texture spriteTexture;
	GGE::Image SpriteImage;
	bool is_init = false;

	static const GGE::Sprite & GetTestSprite(void)
	{
		if (!is_init)
		{
			SpriteImage.create(32, 32, sf::Color::Green);
			spriteTexture.loadFromImage(SpriteImage);
			sprite.setTexture(spriteTexture);
			is_init = true;
		}
		return sprite;
	}
}

using namespace GGE;

std::map<const sf::Event::EventType, EventHandler::Engine::CB>		EventHandler::Engine::_eventTypeToCB_map =
{
	{
		sf::Event::Closed,
		[&](const sf::Event & event, EventHandler::Engine::GameType & game) -> bool { std::cout << "Window closed" << std::endl; game._IsRunning = false; return true; }
	},
	{
		sf::Event::Resized,
		[&](const sf::Event & event, EventHandler::Engine::GameType & game) -> bool { std::cout << "Resized : " << event.size.width << 'x' << event.size.height << std::endl; return true; }
	},
	{
		sf::Event::LostFocus,
		[&](const sf::Event & event, EventHandler::Engine::GameType & game) -> bool { std::cout << "Focus lost" << std::endl; return true; }
	},
	{
		sf::Event::GainedFocus,
		[&](const sf::Event & event, EventHandler::Engine::GameType & game) -> bool { std::cout << "Focus gained" << std::endl; return true; }
	},
	{
		sf::Event::KeyPressed,
		[&](const sf::Event & event, EventHandler::Engine::GameType & game) -> bool { std::cout << "KeyPress : " << event.key.code << std::endl; return true; }
	},
	{
		sf::Event::KeyReleased,
		[&](const sf::Event & event, EventHandler::Engine::GameType & game) -> bool { std::cout << "KeyReleased : " << event.key.code << std::endl; return true; }
	},
	{
		sf::Event::TextEntered,
		[&](const sf::Event & event, EventHandler::Engine::GameType & game) -> bool { std::cout << "Text entered : " << event.text.unicode << " [" << static_cast<char>(event.text.unicode) << ']' << std::endl; return true; }
	},
	{
		sf::Event::MouseWheelMoved,
		[&](const sf::Event & event, EventHandler::Engine::GameType & game) -> bool { std::cout << "Wheel movement : " << event.mouseWheel.delta << " on : " << event.mouseWheel.x << 'x' << event.mouseWheel.y << std::endl; return true; }
	},
	{
		sf::Event::MouseButtonPressed,
		[&](const sf::Event & event, EventHandler::Engine::GameType & game) -> bool
																				{ 
																					std::cout << "MouseButtonPressed : " << event.mouseButton.button << " : " << event.mouseButton.x << 'x' << event.mouseButton.y << std::endl;
																					Entity * e = new Entity(TEST::GetTestSprite(), Point<size_t>(event.mouseButton.x, event.mouseButton.y));
																					e->GetBehavior().AddAction(Entity::State::READY, Behavior<Entity>::Action([&](Entity & e) -> bool { e.MoveTo(Point<size_t>(400, 400)); return true; }));

																					game.AddEntities(e);
																					return true; }
	},
	{
		sf::Event::MouseMoved,
		[&](const sf::Event & event, EventHandler::Engine::GameType & game) -> bool { std::cout << "MouveMoved : " << event.mouseMove.x << 'x' << event.mouseMove.y << std::endl; return true; }
	},
	{
		sf::Event::MouseEntered,
		[&](const sf::Event & event, EventHandler::Engine::GameType & game) -> bool { std::cout << "MouseEntered" << std::endl; return true; }
	},
	{
		sf::Event::MouseLeft,
		[&](const sf::Event & event, EventHandler::Engine::GameType & game) -> bool { std::cout << "MouseLeft" << std::endl; return true; }
	}
};

std::map<const sf::Event::EventType, EventHandler::Debugger::CB>	EventHandler::Debugger::_eventTypeToCB_map =
{
	{
		sf::Event::Closed,
		[&](const sf::Event & event, EventHandler::Debugger::GameType & game) -> bool { std::cout << "Window closed" << std::endl; game._IsRunning = false; return true; }
	},
	{
		sf::Event::Resized,
		[&](const sf::Event & event, EventHandler::Debugger::GameType & game) -> bool { std::cout << "Resized : " << event.size.width << 'x' << event.size.height << std::endl; return true; }
	},
	{
		sf::Event::LostFocus,
		[&](const sf::Event & event, EventHandler::Debugger::GameType & game) -> bool { std::cout << "Focus lost" << std::endl; return true; }
	},
	{
		sf::Event::GainedFocus,
		[&](const sf::Event & event, EventHandler::Debugger::GameType & game) -> bool { std::cout << "Focus gained" << std::endl; return true; }
	},
	{
		sf::Event::KeyPressed,
		[&](const sf::Event & event, EventHandler::Debugger::GameType & game) -> bool { std::cout << "KeyPress : " << event.key.code << std::endl; return true; }
	},
	{
		sf::Event::KeyReleased,
		[&](const sf::Event & event, EventHandler::Debugger::GameType & game) -> bool { std::cout << "KeyReleased : " << event.key.code << std::endl; return true; }
	},
	{
		sf::Event::TextEntered,
		[&](const sf::Event & event, EventHandler::Debugger::GameType & game) -> bool { std::cout << "Text entered : " << event.text.unicode << " [" << static_cast<char>(event.text.unicode) << ']' << std::endl; return true; }
	},
	{
		sf::Event::MouseWheelMoved,
		[&](const sf::Event & event, EventHandler::Debugger::GameType & game) -> bool { std::cout << "Wheel movement : " << event.mouseWheel.delta << " on : " << event.mouseWheel.x << 'x' << event.mouseWheel.y << std::endl; return true; }
	},
	{
		sf::Event::MouseButtonPressed,
		[&](const sf::Event & event, EventHandler::Debugger::GameType & game) -> bool { std::cout << "MouseButtonPressed : " << event.mouseButton.button << " : " << event.mouseButton.x << 'x' << event.mouseButton.y << std::endl; return true; }
	},
	{
		sf::Event::MouseMoved,
		[&](const sf::Event & event, EventHandler::Debugger::GameType & game) -> bool { std::cout << "MouveMoved : " << event.mouseMove.x << 'x' << event.mouseMove.y << std::endl; return true; }
	},
	{
		sf::Event::MouseEntered,
		[&](const sf::Event & event, EventHandler::Debugger::GameType & game) -> bool { std::cout << "MouseEntered" << std::endl; return true; }
	},
	{
		sf::Event::MouseLeft,
		[&](const sf::Event & event, EventHandler::Debugger::GameType & game) -> bool { std::cout << "MouseLeft" << std::endl; return true; }
	}
};

std::map<const sf::Event::EventType, EventHandler::Editor::CB>		EventHandler::Editor::_eventTypeToCB_map =
{
	{
		sf::Event::Closed,
		[&](const sf::Event & event, EventHandler::Editor::GameType & game) -> bool { std::cout << "Window closed" << std::endl; game._IsRunning = false; return true; }
	},
	{
		sf::Event::Resized,
		[&](const sf::Event & event, EventHandler::Editor::GameType & game) -> bool { std::cout << "Resized : " << event.size.width << 'x' << event.size.height << std::endl; return true; }
	},
	{
		sf::Event::LostFocus,
		[&](const sf::Event & event, EventHandler::Editor::GameType & game) -> bool { std::cout << "Focus lost" << std::endl; return true; }
	},
	{
		sf::Event::GainedFocus,
		[&](const sf::Event & event, EventHandler::Editor::GameType & game) -> bool { std::cout << "Focus gained" << std::endl; return true; }
	},
	{
		sf::Event::KeyPressed,
		[&](const sf::Event & event, EventHandler::Editor::GameType & game) -> bool { std::cout << "KeyPress : " << event.key.code << std::endl; return true; }
	},
	{
		sf::Event::KeyReleased,
		[&](const sf::Event & event, EventHandler::Editor::GameType & game) -> bool { std::cout << "KeyReleased : " << event.key.code << std::endl; return true; }
	},
	{
		sf::Event::TextEntered,
		[&](const sf::Event & event, EventHandler::Editor::GameType & game) -> bool { std::cout << "Text entered : " << event.text.unicode << " [" << static_cast<char>(event.text.unicode) << ']' << std::endl; return true; }
	},
	{
		sf::Event::MouseWheelMoved,
		[&](const sf::Event & event, EventHandler::Editor::GameType & game) -> bool { std::cout << "Wheel movement : " << event.mouseWheel.delta << " on : " << event.mouseWheel.x << 'x' << event.mouseWheel.y << std::endl; return true; }
	},
	{
		sf::Event::MouseButtonPressed,
		[&](const sf::Event & event, EventHandler::Editor::GameType & game) -> bool { std::cout << "MouseButtonPressed : " << event.mouseButton.button << " : " << event.mouseButton.x << 'x' << event.mouseButton.y << std::endl; return true; }
	},
	{
		sf::Event::MouseMoved,
		[&](const sf::Event & event, EventHandler::Editor::GameType & game) -> bool { std::cout << "MouveMoved : " << event.mouseMove.x << 'x' << event.mouseMove.y << std::endl; return true; }
	},
	{
		sf::Event::MouseEntered,
		[&](const sf::Event & event, EventHandler::Editor::GameType & game) -> bool { std::cout << "MouseEntered" << std::endl; return true; }
	},
	{
		sf::Event::MouseLeft,
		[&](const sf::Event & event, EventHandler::Editor::GameType & game) -> bool { std::cout << "MouseLeft" << std::endl; return true; }
	}
};
