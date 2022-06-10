#include "req_parse.hpp"
#include "../../srcs/errors/http_code.hpp"
#define MAX_URL_SIZE 5000

namespace preq {

	bool	parse_url( std::string url, std::string & ret_url,std::string & path_info, std::string & query_string )
	{
		if ( url.size() > MAX_URL_SIZE )
			throw HTTPCode414();

		std::string tested_url;
		for ( std::string::iterator it = url.begin(); it != url.end(); ++it )
		{
			if ( *it == '?' )
			{
				while ( it != url.end() && *it != '#')
				{
					query_string += *it;
					++it;
				}
			}
			if ( *it == '#' )
				break;
			tested_url += *it;
		}

		return true;
	}

}