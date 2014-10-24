#ifndef __GGE_ENTITY__
# define __GGE_ENTITY__

# include <SFML\Graphics\RenderWindow.hpp>
# include "Rendering.h"
# include "ObjBinding.h"
# include <functional>


/*
4,5 => 10,2

*/

namespace GGE
{

	template <typename T> struct Point
	{
		Point()
			: _x(0)
			, _y(0)
		{}
		Point(const T x, const T y)
			: _x(x)
			, _y(y)
		{}
		Point(const Point & p)
			: _x(p._x)
			, _y(p._y)
		{}

		inline bool		operator==(const Point & p) const
		{
			return (this->_x == p._x && this->_y == p._y);
		}
		inline Point &	operator=(const Point & p)
		{
			this->_x = p._x;
			this->_y = p._y;
			return *this;
		}
		inline void		operator+=(const Point & p)
		{
			this->_x += p._x;
			this->_y += p._y;
		}
		inline void		operator-=(const Point & p)
		{
			this->_x -= p._x;
			this->_y -= p._y;
		}
		inline void		operator/=(const Point & p)
		{
			this->_x /= p._x;
			this->_y /= p._y;
		}
		inline void		operator*=(const Point & p)
		{
			this->_x *= p._x;
			this->_y *= p._y;
		}

		T _x;
		T _y;
	};

	struct Direction
	{
		Direction(const Point<std::size_t> & from, const Point<std::size_t> & to)
			: _from(from)
			, _to(to)
			, _value()
		{}

		const Point<std::size_t> &	_from;
		const Point<std::size_t> &	_to;
		const Point<float>			_value;
	};

	/*
	Collisionable	-> pas collisionable
	Drawable		-> pas affichable
	*/

	class Entity;

	// Bind differents actions of a class of type 'T' to actions (callback)
	template <class T> class Behaviour : public GCL::Bindable
	{
		using State		= std::size_t;
		using Action	= std::function<bool()>;
		using ActionMap = std::map < State, Action > ;

		Behave() = delete;
		Behaviour(T * obj)
			: _bindedWith(obj)
			, this->_currentState(GetDefaultState())
		{}

		bool	Do(void)
		{
			ActionMap::iterator it;
			if ((it = _actions.find(_currentState)) == _actions.end())
				throw std::exception("[Error] : Not mapped state");
			return (*it)();
		}

		void	AddAction(const State state, Action & action)
		{
			this->_actions[state] = action;
		}
		void	DeleteAction(const State state)
		{
			ActionMap::iterator it;
			if ((it = _actions.find(state)) != _actions.end())
				_actions.erase(it);
		}

		virtual inline void	GetDefaultState(void) = 0;				// Initiale state
		inline void			SetState(const State state)
		{
			this->_currentState = state;
		}

	protected:
		State				_currentState;
		std::vector<State>	_states;
		ActionMap			_actions;
	};

	class Entity
	{
	public:

		void	Draw(sf::RenderWindow & window)
		{
			window.draw(_sprite);
		}
	protected:
		Point<std::size_t>	_pos;	// top-left point
		Point<std::size_t>	_size;	// _pos + size => bot-right point
		Sprite				_sprite;
	};
}

#endif // __GGE_ENTITY__
