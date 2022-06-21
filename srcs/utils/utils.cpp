#include <iostream>
#include <string>
#include <sstream>
#include <map>
#include <vector>
#include "../errors/http_code.hpp"

bool	is_full( std::string s, std::string charset )
{
	for ( std::string::iterator it = s.begin(); it != s.end(); ++it )
	{
		if ( charset.find(*it) == std::string::npos )
			return false;
	}
	return true;
}

bool	is_full_hexa( std::string s )
{
	return is_full( s, "0123456789abcdefABCDEF");
}

bool	check_line( std::string str ) {

	return ( str.size() >= 2 
				&& str[ str.size() - 1 ] == '\n'
				&& str[ str.size() - 2] == '\r'
			);
}

bool	is_space( char c ) {

	return ( c == ' ' || c == '\t' );
}

bool	check_and_trunc_line( std::string *line ) {

	if ( check_line( *line ) )
	{
		*line = line->substr( 0, line->size() - 2 );
		return true;
	}
	return false;
}

std::string test_through_str( std::string &str, bool (*rule)(std::string) )
{
	std::string s;

	for ( std::string::iterator it = str.begin(); it != str.end(); ++it )
	{
		s += *it;
		if ( rule( s ) )
			break ;
	}
	return s;
}

int hexToInt(std::string hex)
{
    std::stringstream stream;
    stream << std::hex << hex;
    int result;
    stream >> result;
    return result;
}

bool is_more_ll( const std::string &str )
{
	if ( str.size() > 19 )
		return true;
	if ( str.size() == 19 )
		return str > std::string("9223372036854775807");
	return false;
}

std::map<std::string, std::string> parse_params( std::vector<std::string>::iterator begin, std::vector<std::string>::iterator end )
{
	std::map<std::string, std::string> res;
	for ( std::vector<std::string>::const_iterator it = begin; it != end; ++it )
	{
		//first split in half
		std::size_t find = it->find( "=" );
		if ( find == std::string::npos )
			throw HTTPCode400();

		std::string name = it->substr( 0, find );
		std::string val = it->substr( find + 1, it->size() );
		//then verify quotes
		if ( val[0] == '"' )
		{
			if ( val.size() < 2 )
				throw HTTPCode400();
			if ( val[ val.size() - 1 ] != '"' )
				throw HTTPCode400();
			val = val.substr( 1, val.size() - 2 );
		}
		//then we insert
		res.insert( std::pair<std::string, std::string>( name, val ) );
	}
	return res;
}

#include "../../request_parsing/srcs/req_parse.hpp"

std::vector<std::string>	split_params( const std::string & raw )
{
	std::vector<std::string> params = preq::split_str_c( raw, ';' );

	for ( std::vector<std::string>::iterator it = params.begin(); it != params.end(); ++it )
		*it = preq::trim( *it, &preq::is_space );
	
	return params;
}

