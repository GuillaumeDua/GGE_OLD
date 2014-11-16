#ifndef __GGE_ENTITY__
# define __GGE_ENTITY__

# include <SFML\Graphics\RenderWindow.hpp>
# include "Rendering.h"
# include "GCL/ObjBinding.h"
# include "behavior.h"
# include "Geometry.h"

# include "Serializable.h"

using namespace GGE::Geometry;
using namespace GCL;

namespace GGE
{
	/*
	Collisionable	-> pas collisionable
	Drawable		-> pas affichable

	=> Policy

	*/

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

	// [Todo] : Recycle
	// static GetRecycle
	// override new Entity / new [] Entity
	class Entity : public Serializable
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
		}					_type;
		inline const Type					GetType(void) const
		{
			return this->_type;
		}

		using CollisionAction = std::function<void(Entity &, Entity &)>;
		using CollisionActionsMap = std::map < Entity::Type, CollisionAction >; // [?] std::multimap [?]

		CollisionActionsMap					_collisionsActionsMap;
		void								OnCollision(Entity & with)
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

		//std::ostream &						operator<<(std::ostream & os)
		//{
		//	this->_pos.Dump(os);
		//	return os;
		//}

		void								Behave()
		{
			this->_behaviour.Do();
		}
		void								Draw(sf::RenderWindow & window)
		{
			_sprite.setPosition(static_cast<float>(_pos._x), static_cast<float>(_pos._y));
			window.draw(_sprite);
		}
		inline Behavior<Entity> &			GetBehavior(void)
		{
			return this->_behaviour;
		}
		inline void							SetTexture(const Texture & texture)
		{
			this->_sprite.setTexture(texture);
		}
		void								MoveTo(const Direction & d)
		{

		}
		void								MoveTo(const Point<size_t> & coord)
		{

		}
		static const State					GetDefaultState(void)
		{
			return READY;
		}
		inline const Point<std::size_t>	&	GetPosition() const
		{
			return this->_pos;
		}

		bool								_needPositionRefresh;		// Need position re-mapping ?

	protected:
		Behavior<Entity>					_behaviour = Behavior<Entity>(*this);

		Point<std::size_t>					_pos;			// top-left point
		Point<std::size_t>					_size;			// _pos + size => bot-right point
		Sprite								_sprite;
		size_t								_movementSpeed;

	private :
		// [Todo] : Serializatio format
		virtual bool						_Serialize(std::ostream & os)
		{
			return true;
		}
		virtual bool						_Unserialize(std::istream & is)
		{
			return true;
		}
	};
}

#endif // __GGE_ENTITY__
