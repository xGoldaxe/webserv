#include "webserv.hpp"

template < class exception >
std::string	go_through_it_until
( 
	std::vector<std::string> values, 
	bool (*rule)(std::string, std::string&),
	exception
)
{
	std::string res;
	for ( std::vector<std::string>::iterator it = values.begin(); it !=values.end(); ++it )
	{
		if ( rule( *it, res ) )
			return res;
	}
	throw exception();
}
