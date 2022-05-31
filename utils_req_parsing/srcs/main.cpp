#include "req_parse.hpp"

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
			break ;
	}

	return res;
}

bool	check_and_trunc_line( std::string *line ) {

	if ( line->size() >= 2 && (*line)[ line->size() - 1 ] == '\n'
			&& (*line)[ line->size() - 2] == '\r' )
	{
		*line = line->substr( 0, line->size() - 2 );
		return true;
	}
	return false;
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

/* trim */
std::string	left_trim( std::string str, bool (*rule)(char) ) {

	int i = 0;
	for ( std::string::iterator it = str.begin(); it != str.end(); ++it )
	{
		if ( rule(*it) == false )
		{
			// std::cout << str.substr( i, str.size() - 1 ) << "l" << std::endl;
			return str.substr( i, str.size() - 1 );
		}
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
		{
			// std::cout << "r " << str.substr( i, str.size() - 1 ) << std::endl;
			return str.substr( 0, str.size() - i );
		}
		++i;
	}
	// cant trim an empty string
	throw std::exception();
	return str;
}
std::string	trim( std::string str, bool (*rule)(char) ) {

	return ( left_trim( right_trim( str, rule ), rule ) );
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

std::map<std::string, std::string>	get_headers_req( int fd ) {

	std::map<std::string, std::string> res;

	while ( 1 )
	{
		std::string header_field = get_line_req( fd );
		//invalid end of file
		if ( check_and_trunc_line( &header_field ) == false )
			throw std::exception();
		// empty line, that prepare for body
		if ( header_field.size() == 0 )
			break ;
		
		const std::basic_string<char>::size_type point_index = header_field.find(':');
		if ( point_index == std::string::npos || point_index + 1 >= header_field.size() ) // invalid header field
			throw std::exception();

		std::string	field_name = header_field.substr( 0, point_index );
		std::string	field_value = header_field.substr( point_index + 1, header_field.size() - 1 );
		field_value = trim( field_value, &is_space );
		res.insert( std::pair<std::string, std::string>( field_name, field_value ) );
	}
	return res;
}

/* during the parsing at any time, an exception could happened. We just have to
catch it and send back an error 400 response */

int main( void ) {

	try {
		// first line
		std::string start_line = get_line_req( STDIN_FILENO );
		if ( check_and_trunc_line( &start_line ) == false )
			throw std::exception();
		std::vector<std::string> parsed_first_line = parse_start_line( start_line );
		
		// headers
		std::map<std::string, std::string> headers = get_headers_req( STDIN_FILENO );

		// body
		// ...

		print_or_store( parsed_first_line , headers);
	}
	catch(const std::exception& e) {

		std::cout << "error 400, bad request" << std::endl;
	}

	return (0);
}