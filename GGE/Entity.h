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


		std::ostream & Dump(std::ostream & os) const
		{
			os << '(' << this->_x << ',' << this->_y << ')';
			return os;
		}
		std::ostream & operator<<(std::ostream & os) const
		{
			return this->Dump();
		}

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

	// [Todo] : IBehaviour

	// Bind differents actions of a class of type 'T' to actions (callback)
	template <class T> class Behaviour
	{
	public:
		typedef		typename	T::State mState;
		using		Action		= std::function < bool(T&) > ;
		using		ActionMap	= std::multimap < mState, Action > ;
		using		Element		= std::pair < mState, Action > ;

		//Behave() = delete;
		Behaviour(T & obj)
			: _entity(obj)
			, _currentState(T::GetDefaultState())
		{}

		template <typename T_Operator> void		Iterate(const mState state, T_Operator & op)
		{
			std::pair<ActionMap::iterator, ActionMap::iterator>	range;
			range = this->_actions.equal_range(_currentState);
			for (ActionMap::iterator it = range.first; it != range.second; ++it)
				if (!(op(it->second)))
					std::cerr << "[Error] : Behaviour::Iterator : callback returned false" << std::endl;
		}
		bool									Do(void)
		{
			// Map style :
			/*ActionMap::iterator it;
			if ((it = _actions.find(_currentState)) == _actions.end())
				throw std::exception("[Error] : Not mapped state");

			return (it->second)(this->_entity);*/

			// Multimap style :
			std::pair<ActionMap::iterator, ActionMap::iterator>	range;
			range = this->_actions.equal_range(_currentState);
			for (ActionMap::iterator it = range.first; it != range.second; ++it)
				if (!((it->second)(this->_entity)))
					return false;
			return true;
		}
		void									Reset(void)
		{
			this->_currentState = T::GetDefaultState();
			this->_actions.clear();
		}
		void									operator-=(const mState state)
		{
			this->RemoveAction(state);
		}
		void									operator+=(Element && p)
		{
			this->_actions.insert(std::move(p));
		}
		void									AddAction(const mState state, Action & action)
		{
			this->_actions.insert(std::move(Element(state, action)));
		}
		void									RemoveAction(const mState state)
		{
				_actions.erase(state);
		}
		void									SetActionsMap(const ActionMap & actions)
		{
			this->_actions = actions;
		}
		inline const mState						GetState(void) const
		{
			return this->_currentState;
		}
		inline void								SetState(const mState state)
		{
			this->_currentState = state;
		}

		void									Dump(std::ostream & os = std::cout)
		{
			os << "Behavior binded to " << &_entity << " has actions mapped to its states : " << std::endl;
			for (auto & e : this->_actions)
				os << "\-[" << e.first << " : " << &(e.second) << ']' << std::endl;
		}

	protected:
		T &										_entity;
		mState									_currentState;
		ActionMap								_actions;
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
		enum Type
		{
			MAP_ELEMENT
			, EVENT_TRIGGER
			, 
		}	_type;
		inline const Type	GetType(void) const
		{
			return this->_type;
		}


		// [Todo] : IBehaviour
		using CollisionAction = std::function<void(Entity &, Entity &)>;
		using CollisionActionsMap = std::map < Entity::Type, CollisionAction >;

		CollisionActionsMap	_collisionsActionsMap;

		void	OnCollision(Entity & with)
		{
			CollisionActionsMap::iterator it = this->_collisionsActionsMap.find(with.GetType());
			if (it == this->_collisionsActionsMap.end())
				return;
			(it->second)(*this, with);
		}

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

		std::ostream &	operator<<(std::ostream & os)
		{
			this->_pos.Dump(os);
			return os;
		}

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
		inline const Point<std::size_t>	& GetPosition() const
		{
			return this->_pos;
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
