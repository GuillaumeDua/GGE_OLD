#ifndef __GGE_GEOMETRY__
# define __GGE_GEOMETRY__

# include <functional>

namespace GGE
{
	namespace Geometry
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
	}
}

#endif // __GEE_GEOMETRY__