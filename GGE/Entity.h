#ifndef __GGE_ENTITY__
# define __GGE_ENTITY__

# include <SFML\Graphics\RenderWindow.hpp>
# include "Rendering.h"
# include "GCL/ObjBinding.h"
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

	=> Policy

	*/

	// Bind differents actions of a class of type 'T' to actions (callback)
	template <class T> class Behaviour
	{
	public :
		typedef		typename	T::State mState;
		using		Action		= std::function<bool(T&)>;
		using		ActionMap	= std::map < mState, Action >;

		//Behave() = delete;
		Behaviour(T & obj)
			: _entity(obj)
			, _currentState(T::GetDefaultState())
		{}

		bool						Do(void)
		{
			ActionMap::iterator it;
			if ((it = _actions.find(_currentState)) == _actions.end())
				throw std::exception("[Error] : Not mapped state");

			return (it->second)(this->_entity);
		}
		void						Reset(void)
		{
			this->_currentState = T::GetDefaultState();
			this->_actions.clear();
		}
		
		void						AddAction(const mState state, Action & action)
		{
			this->_actions[state] = action;
		}
		void						RemoveAction(const mState state)
		{
			ActionMap::iterator it;
			if ((it = _actions.find(state)) != _actions.end())
				_actions.erase(it);
		}
		void						SetActionsMap(const ActionMap & actions)
		{
			this->_actions = actions;
		}

		inline const mState			GetState(void) const
		{
			return this->_currentState;
		}
		inline void					SetState(const mState state)
		{
			this->_currentState = state;
		}

	protected:
		T &							_entity;
		mState						_currentState;
		ActionMap					_actions;
	};


	/*
	
	void	       		Moveable::MoveInDirection(const RatiolizedVector2D & v)
	{
	  const float s		= (this->_speed * this->_app->GetFrameTime());


	  this->_directionX	= (v._x > 0 ? true : false);
	  this->_x		+= v._x * s;
	  this->_y		+= v._y * s;
	}

	void		       	Moveable::MoveInDirection(const Point<float> & p)
	{
	  const float s		= (this->_speed * this->_app->GetFrameTime());
	  const float ratio	= RatiolizedVector2D::GetRatio(p._x, p._y);

	  this->_directionX	= (p._x > this->_x ? true : false);
	  this->_x		+= (p._x / ratio) * s;
	  this->_y		+= (p._y / ratio) * s;
	}


	void   			Moveable::MoveTo(float dx, float dy)
	{
	  const float s		= (this->_speed * this->_app->GetFrameTime());
	  const float ratio	= RatiolizedVector2D::GetRatio(dx, dy);

	  if (dx < this->_x)
		{
		  this->_x += (dx / ratio) * s;
		  if (this->_x < dx) this->_x = dx;
		  this->_directionX	= false;
		}
	  else if (dx > this->_x)
		{
		  this->_x += (dx / ratio) * s;
		  if (this->_x > dx) this->_x = dx;
		  this->_directionX	= true;
		}

	  if (dy < this->_y)
		{
		  this->_y += (dy / ratio) * s;
		  if (this->_y < dy) this->_y = dy;
		}
	  else if (dy > this->_y)
		{
		  this->_y += (dy / ratio) * s;
		  if (this->_y > dy) this->_y = dy;
		}
	}

	*/

	class Entity
	{
	public:
		enum State
		{
			READY,
			IN_USE,
			FROZEN,		// Skip all status check
			GARBAGABLE,	// Can be recycle by a garbage collector
		};

		Entity(const Sprite & sprite,
			Point<size_t> pos)
			: _needPositionRefresh(true)
			, _sprite(sprite)
			, _pos(pos)
		{}
		Entity() = delete;
		~Entity(){}

		// [Todo] : Recycle
		// static GetRecycle
		// override new Entity / new [] Entity

		void	Behave()
		{
			this->_behaviour.Do();
		}
		void	Draw(sf::RenderWindow & window)
		{
			_sprite.setPosition(static_cast<float>(_pos._x), static_cast<float>(_pos._y));
			window.draw(_sprite);
		}
		inline Behaviour<Entity> &	GetBehaviour(void)
		{
			return this->_behaviour;
		}
		inline void	SetTexture(const Texture & texture)
		{
			this->_sprite.setTexture(texture);
		}
		void	MoveTo(const Direction & d)
		{

		}
		void	MoveTo(const Point<size_t> & coord)
		{

		}
		static const State	GetDefaultState(void)
		{
			return READY;
		}
		bool	_needPositionRefresh;		// Need position re-mapping ?

	protected:
		Behaviour<Entity>	_behaviour = Behaviour<Entity>(*this);

		Point<std::size_t>	_pos;			// top-left point
		Point<std::size_t>	_size;			// _pos + size => bot-right point
		Sprite				_sprite;
		size_t				_movementSpeed;
	};
}

#endif // __GGE_ENTITY__
