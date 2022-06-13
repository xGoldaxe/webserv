#include "req_parse.hpp"
#include <time.h>

namespace preq {

	time_t timer;

	bool	check_line( std::string str ) {

		return ( str.size() >= 2 
					&& str[ str.size() - 1 ] == '\n'
					&& str[ str.size() - 2] == '\r'
				);
	}

	bool	check_and_trunc_line( std::string *line ) {

		if ( check_line( *line ) )
		{
			*line = line->substr( 0, line->size() - 2 );
			return true;
		}
		return false;
	}

	bool	is_space( char c ) {

		return ( c == ' ' || c == '\t' );
	}
		
	std::vector<std::string>	read_until( std::string & data, bool (*rule)(std::string) ) {

		std::string actual;
		std::vector<std::string> res;

		for ( std::string::iterator it = data.begin(); it != data.end(); ++it )
		{
			actual += *it;
			if ( rule(actual) )
			{
				res.push_back( actual );
				actual = "";
			}
		}
		return res;
	}

	std::vector<std::string>	split_str( std::string str ) {

		std::vector<std::string> splitted_str;

		std::stringstream stream_str( str );
		std::string buffer_str;
		while ( std::getline(stream_str, buffer_str, ' ') )
			splitted_str.push_back(buffer_str);
		return	splitted_str;
	}

	/* trim */
	std::string	left_trim( std::string str, bool (*rule)(char) ) {

		int i = 0;
		for ( std::string::iterator it = str.begin(); it != str.end(); ++it )
		{
			if ( rule(*it) == false )
				return str.substr( i, str.size() );
			++i;
		}
		// trim everything
		return "";
	}
	std::string	right_trim( std::string str, bool (*rule)(char) ) {

		int i = 0;
		for ( std::string::reverse_iterator rit = str.rbegin(); rit != str.rend(); ++rit )
		{
			if ( rule(*rit) == false )
				return str.substr( 0, str.size() - i );
			++i;
		}
		// trim everything
		return "";
	}
	std::string	trim( std::string str, bool (*rule)(char) ) {

		return ( left_trim( right_trim( str, rule ), rule ) );
	}
}