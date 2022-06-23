#include "req_parse.hpp"
#include <iterator>
#include "../../srcs/errors/http_code.hpp"
#include "../../srcs/utils/string.hpp"
#include "../../srcs/utils/utils.hpp"

bool	preq::is_full_hexa( std::string s )
{
	std::string hexa = "0123456789abcdefABCDEF";
	for ( std::string::iterator it = s.begin(); it != s.end(); ++it )
	{
		if ( hexa.find(*it) == std::string::npos )
			return false;
	}
	return true;
}

std::string preq::replace_percent( std::string url )
{
	std::string res;
	for ( std::string::iterator it = url.begin(); it != url.end(); ++it )
	{
		if ( *it == '%' )
		{
			++it;
			if ( std::distance( it, url.end() ) < 2 )
				throw HTTPCode400();
			if ( is_full_hexa( std::string( it, it + 2 ) ) == false )
				throw HTTPCode400();
			res += hexToInt( std::string( it, it + 2 ) );
			++it;
		}
		else
			res += *it;
	}
	return res;
}

bool	preq::parse_url( std::string url, std::string & ret_url,std::string & path_info, std::string & query_string, size_t max_url_size )
{
	(void)path_info;
	if ( url.size() > max_url_size )
		throw HTTPCode414();

	url = replace_percent( url );

	std::string res_url;
	for ( std::string::iterator it = url.begin(); it != url.end(); ++it )
	{
		if ( *it == '?' )
		{
			++it;
			while ( it != url.end() && *it != '#')
			{
				query_string += *it;
				++it;
			}
			break ;
		}
		if ( *it == '#' )
			break;
		res_url += *it;
	}
	ret_url = res_url;
	return true;
}