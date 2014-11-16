#ifndef __GGE_SCREEN__
# define __GGE_SCREEN__

# include <vector>
# include <functional>

# include "Rendering.h"
# include "Entity.h"
# include "Geometry.h"
# include "Serializable.h"

using namespace GCL;
using namespace GGE::Geometry;

// [Todo] : Serialization format

namespace GGE
{
	// Describe a screen in the game
	// Background, window, and not human entities (AI, Events, etc ...)
	struct Screen : public Serializable
	{
		Screen(const std::ostream & os)
		{}
		~Screen()
		{
			for (auto & e : this->_entities)
				delete e;
		}

		void									Activate()
		{
			this->_isActive = true;
			// Load entities
		}
		void									Desactivate()
		{
			this->_isActive = false;
			// Unload entities
		}

		void									ManipulateEntities(const std::function<void (Entity&)> & cb)
		{
			for (auto & e : this->_entities)
				cb(*e);
		}
		void									Display(RenderWindow & renderWindow)
		{
			for (auto & e : this->_entities)
				e->Draw(renderWindow);
		}
		void									BehaveEntity()
		{
			for (auto & e : this->_entities)
				e->Behave();
		}

		inline const std::vector<Entity *> &	GetEntities(void) const
		{
			return this->_entities;
		}

	protected:
		bool									_isActive = false;
		std::vector<Entity *>					_entities;
	};

	// All screens in the game
	struct ScreensManager
	{
		ScreensManager(std::vector<Screen *> & screens)
			: _screens(screens)
			, _activeScreen(screens.empty() ? 0x0 : screens.at(0))
		{}
		~ScreensManager()
		{
			this->Clean();
		}

		inline void				SetActiveScreen(const size_t index)
		{
			if (this->_activeScreen != 0x0)
				this->_activeScreen->Desactivate();
			this->_activeScreen = this->_screens.at(index);
			this->_activeScreen->Activate();
		}
		inline void				SetScreens(std::vector<Screen *> screens)
		{
			this->_screens = screens;
		}

		void					Behave()
		void					Display()
		{
			if (this->_activeScreen != 0x0)
				this->_activeScreen->Display(*(this->_renderWindow));
			else std::cerr << "[Warning] : ScreensManager::Display : No screen is active" << std::endl;
		}
		void					Clean()
		{
			for (auto & e : this->_screens)
				delete e;
			this->_screens.clear();
		}

	protected:
		RenderWindow *			_renderWindow;
		std::vector<Screen *>	_screens;		// All screens
		Screen *				_activeScreen;	// The screen that is in use
	};
}

#endif // __GGE_SCREEN__