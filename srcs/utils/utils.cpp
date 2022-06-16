#include <iostream>
#include <string>
#include <sstream>

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

