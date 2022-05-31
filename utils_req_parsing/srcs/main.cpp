#include <unistd.h>
#include <iostream>
#include <string.h>
#include <vector>
#include <sstream>
#include <iomanip>
#include <sys/types.h>
#include <sys/socket.h>

std::string get_line_req( int fd )
{
	char 			buffer[256];
	std::string		res;

	bzero( buffer, 256 );
	int n = 1;
	while ( n == 1 )
	{
		bzero( buffer, 255 );
		n = read( fd, buffer, 1 );
		buffer[n] = '\0';
		res += buffer;
		if ( res.size() >= 2 && res[ res.size() - 1 ] == '\n' && res[ res.size() - 2] == '\r' )
		{
			res = res.substr( 0, res.size() - 2 );
			break ;
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

bool	is_space( char c ) {

	return ( c == ' ' || c == '\t' );
}

std::vector<std::string>	parse_start_line( std::string raw_line ) {

	std::vector<std::string> splited = split_str( raw_line );
	if ( splited.size() != 3 )
		throw std::exception();
	for ( int i = 0; i < 3; ++i )
	{
		if ( splited[i].size() < 1 )
			throw std::exception();
	}
	return splited;
}

void	print_or_store( std::vector<std::string> parsed_first_line ) {

	std::cout << "METHOD: " << parsed_first_line[0] << std::endl;
	std::cout << "PATH: " << parsed_first_line[1] << std::endl;
	std::cout << "VERSION: " << parsed_first_line[2] << std::endl;
}

int main( void ) {

	try {
		std::string start_line = get_line_req( STDIN_FILENO );
		std::vector<std::string> parsed_first_line = parse_start_line( start_line );

		print_or_store( parsed_first_line );
	}
	catch(const std::exception& e) {

		std::cout << "error 400, bad request" << std::endl;
	}

	return (0);
}