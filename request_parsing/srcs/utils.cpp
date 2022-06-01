#include "req_parse.hpp"

std::string	read_until( int fd, bool (*rule)(std::string) ) {

	char 			buffer[2];
	std::string		res;

	bzero( buffer, 2 );
	int n = 1;
	while ( n == 1 && rule( res ) == false )
	{
		bzero( buffer, 2 );
		n = read( fd, buffer, 1 );
		res += buffer;
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
	// cant trim an empty string
	throw std::exception();
	return str;
}
std::string	right_trim( std::string str, bool (*rule)(char) ) {

	int i = 0;
	for ( std::string::reverse_iterator rit = str.rbegin(); rit != str.rend(); ++rit )
	{
		if ( rule(*rit) == false )
			return str.substr( 0, str.size() - i );
		++i;
	}
	// cant trim an empty string
	throw std::exception();
	return str;
}
std::string	trim( std::string str, bool (*rule)(char) ) {

	return ( left_trim( right_trim( str, rule ), rule ) );
}