#ifndef __GEE_BEHAVIOR__
# define __GEE_BEHAVIOR__

#include <iostream>
#include <ctime>
#include <ratio>
#include <chrono>

namespace GGE
{
	template <class T_Entity, typename T_Key, typename T_CB_Signature> class IBehavior
	{
	public:
		using		Action = T_CB_Signature;
		using		ActionMap = std::multimap < T_Key, Action >;
		using		Element = std::pair < T_Key, Action >; // value_type

		IBehavior(T_Entity & obj)
			: _entity(obj)
		{}

		template <typename T_Operator> void		Iterate(const T_Key key, T_Operator & op)
		{
			std::pair<ActionMap::iterator, ActionMap::iterator>	range;
			range = this->_actions.equal_range(_currentState);
			for (ActionMap::iterator it = range.first; it != range.second; ++it)
				if (!(op(it->second)))
					std::cerr << "[Error] : IBehavior::Iterate : callback returned false" << std::endl;
		}
		typename ActionMap::iterator &					At(const T_Key key)
		{
			return this->_actions.equal_range(key);
		}
		virtual bool							Do(void) = 0;
		virtual void							Reset(void)
		{
			this->_actions.clear();
		}
		void									operator-=(const T_Key key)
		{
			this->RemoveAction(key);
		}
		void									operator+=(Element && p)
		{
			this->_actions.insert(std::move(p));
		}
		void									AddAction(const T_Key key, Action & action)
		{
			this->_actions.insert(std::move(Element(key, action)));
		}
		void									RemoveAction(const T_Key key)
		{
			_actions.erase(key);
		}
		void									SetActionsMap(const ActionMap & actions)
		{
			this->_actions = actions;
		}

		void									Dump(std::ostream & os = std::cout)
		{
			os << "Behavior binded to " << &_entity << " has actions mapped to its states : " << std::endl;
			for (auto & e : this->_actions)
				os << "\t-[" << e.first << " : " << &(e.second) << ']' << std::endl;
		}

	protected:
		T_Entity &								_entity;
		ActionMap								_actions;
	};

	template <typename T> struct BehaviorTypeAdaptor
	{
		using key	= typename T::State;
		using value	= std::function<bool(T&)>;

		typedef IBehavior<T, key, value> BehaviorType;

		/*static IBehavior<T, key, value> Make(void)
		{
			return IBehavior<T, key, value>();
		}*/
	};

	// Bind differents actions of a class of type 'T' to actions (callback)
	template <class T> class Behavior : public BehaviorTypeAdaptor<T>::BehaviorType
	{
	public:
		using mState = typename BehaviorTypeAdaptor<T>::key;
		using Action = typename BehaviorTypeAdaptor<T>::value;
		using ActionMap = std::map < mState, Action >;

		//Behave() = delete;
		Behavior(T & obj)
			: BehaviorTypeAdaptor<T>::BehaviorType(obj)
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
			for (auto const & e : this->_actions)
				os << "\t-[" << e.first << " : " << &(e.second) << ']' << std::endl;
		}

	protected:
		mState									_currentState;
	};

	// To calculate cooldown, or such things
	template <class T_Entity> class TimedBehavior : public IBehavior < T_Entity, std::chrono::high_resolution_clock::time_point, std::function<bool(T_Entity&)> >
	{
	public:

		bool											Do(void)
		{
			for (ActionMap::iterator it = this->_actions.lower_bound(_lastRefresh = high_resolution_clock::now()); it != this->_actions.end(); ++it)
				if (!((it->second)(this->_entity)))
					return false;
			return true;
		}
		void											Dump(std::ostream & os = std::cout)
		{
			os << "Behavior binded to " << &_entity << " has actions mapped to its states : " << std::endl;
			for (auto const & e : this->_actions)
				os << "\t-[" << e.first << " : " << &(e.second) << ']' << std::endl;
		}


	protected:
		std::chrono::high_resolution_clock::time_point	_lastRefresh;
	};

	//RecurrentTimedBehavior
}

#endif // __GEE_BEHAVIOR__