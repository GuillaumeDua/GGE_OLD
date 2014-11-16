#ifndef __GLC_SERIALIZABLE__
# define __GLC_SERIALIZABLE__

# include <string>
# include <fstream>
# include <iostream>

namespace GCL
{
	class Serializable
	{
	public:
		Serializable()
		{}
		virtual ~Serializable()
		{}

		bool			Serialize(const std::string & path)
		{
			std::ofstream ofs(path);
			return (ofs.is_open() && this->_Serialize(ofs));
		}
		bool			Unserialize(const std::string & path)
		{
			std::ifstream ifs(path);
			return (ifs.is_open() && this->_Unserialize(ifs));
		}
		std::ostream &	operator<<(std::ostream & os)
		{
			this->_Serialize(os);
		}
		std::istream &	operator>>(std::istream & is)
		{
			this->_Unserialize(is);
		}

	protected:
		virtual bool	_Serialize(std::ostream &) = 0;
		virtual bool	_Unserialize(std::istream &) = 0;
	};
}

#endif // __GLC_SERIALIZABLE__