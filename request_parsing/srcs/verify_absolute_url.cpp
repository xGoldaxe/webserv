#include "req_parse.hpp"
#include "../../srcs/errors/http_code.hpp"

bool	preq::verify_absolute_url( std::string url ) {

	std::string tested_url;
	for ( std::string::iterator it = url.begin(); it != url.end(); ++it )
	{
		if ( *it == '?' || *it == '#')
			break;
		tested_url += *it;
	}

	int nb_sub = 0;
	int nb_regular = 0;
	int nb_back = 0;
	std::string	actual;
	for ( std::string::iterator it = tested_url.begin(); it != tested_url.end(); ++it )
	{
		if ( *it == '/' && actual.size() > 0 ) /* we ignore ////qwe///qwe//ewq case */
		{
			actual != ".." ? ++nb_regular : ++nb_back;
			++nb_sub;
			actual = "";
		}
		else if ( *it != '/' )
			actual += *it;
	}
	if ( actual.size() != 0 ) // case where there is no / at end
	{
		actual != ".." ? ++nb_regular : ++nb_back;
		++nb_sub;
	}

	if ( nb_sub == 0 && !( tested_url.size() > 0 && tested_url[0] == '/' ) )
		return false;
	if ( tested_url.size() == 0 || tested_url[0] != '/' )
		return false;
	if ( nb_back > nb_regular )
		return false;

	return true;
}