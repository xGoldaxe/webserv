#pragma once

#include "req_parse.hpp"
#include <time.h>
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include "../../srcs/errors/http_code.hpp"

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
		
	std::vector<std::string>	read_until( const std::string & data, bool (*rule)(std::string) ) {

		std::string actual;
		std::vector<std::string> res;

		for ( std::string::const_iterator it = data.begin(); it != data.end(); ++it )
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

	std::vector<std::string>	split_str_c( const std::string & str, char c ) {

		std::vector<std::string> splitted_str;

		std::stringstream stream_str( str );
		std::string buffer_str;
		while ( std::getline(stream_str, buffer_str, c) )
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

	std::map<std::string, std::string>	get_headers_req( std::vector<std::string>::iterator begin, std::vector<std::string>::iterator end ) {

		std::map<std::string, std::string> res;

		for ( std::vector<std::string>::iterator it = begin; it != end; ++it )
		{
			std::string header_field = *it;
			//invalid end of file
			if ( check_and_trunc_line( &header_field ) == false )
				throw HTTPCode400();
			
			if ( header_field.size() == 0 ) //empty line seperator reached
				break ;

			const std::basic_string<char>::size_type point_index = header_field.find(':');
		
			if ( point_index == std::string::npos || point_index + 1 >= header_field.size() ) // invalid header field
				throw HTTPCode400();

			std::string	field_name = header_field.substr( 0, point_index );
			std::string	field_value = header_field.substr( point_index + 1, header_field.size() - 1 );
			field_value = trim( field_value, &is_space );
			if ( field_value.size() == 0 )
				throw HTTPCode400();
			res.insert( std::pair<std::string, std::string>( field_name, field_value ) );
		}
		return res;
	}
}