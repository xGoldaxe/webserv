#ifndef UTILS_HPP
#	define UTILS_HPP

#include <iostream>
#include <string>
#include <vector>

bool	is_full_hexa( std::string s )
{
	std::string hexa = "0123456789abcdefABCDEF";
	for ( std::string::iterator it = s.begin(); it != s.end(); ++it )
	{
		if ( hexa.find(*it) == std::string::npos )
			return false;
	}
	return true;
}

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
	
std::vector<std::string>	read_until( std::string & data, bool (*rule)(std::string), int max = -1 ) {

	std::string actual;
	std::vector<std::string> res;

	int i = 0;
	for ( std::string::iterator it = data.begin(); it != data.end(); ++it )
	{
		actual += *it;
		if ( rule(actual) )
		{
			res.push_back( actual );
			actual = "";
			++i;
			if ( max != -1 && i >= max )
				break ;
		}
	}
	return res;
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

#endif