#include "req_parse.hpp"

using namespace preq;

template < typename t_size >
t_size	check_str_size_memory( bool mode, t_size value = 0 ) {

	static t_size	stored_value = 0;
	if ( mode == true )
		stored_value = value;
	return stored_value;
}

bool	check_str_size( std::string str ) {

	return ( str.size() >= check_str_size_memory<size_t>( false ) );
}

bool	check_line( std::string str ) {

	return ( str.size() >= 2 
				&& str[ str.size() - 1 ] == '\n'
				&& str[ str.size() - 2] == '\r'
			);
}

std::string get_line_req( int fd ) {

	return read_until( fd, &check_line );
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

std::string	read_body_req( int fd, size_t body_length )
{
	check_str_size_memory<size_t>( true, body_length );
	return read_until( fd, &check_str_size );
}

/* during the parsing at any time, an exception could happened. We just have to
catch it and send back an error 400 response */

int parse_request( int sock_fd ) {

	try {
		// first line
		std::string start_line = get_line_req( sock_fd );
		if ( check_and_trunc_line( &start_line ) == false )
			throw std::exception();
		std::vector<std::string> parsed_first_line = parse_start_line( start_line );
		
		// headers
		std::map<std::string, std::string> headers = get_headers_req( sock_fd );
		
		// body
		/* in case the user doesnt chose a body size, we put a default one to zero
		, spec doesnt like it but it's ok */
		headers.insert( std::pair<std::string, std::string>( "Content-Length", "0" ) );
		// may throw an error if content-length is not defined
		std::string str_body_length = headers.at( "Content-Length" );
		char	*end_ptr;
		size_t	body_length = strtoul( str_body_length.c_str(), &end_ptr, 10 );
		std::string raw_body = read_body_req( sock_fd, body_length );

		if ( raw_body.size() != body_length )
			throw std::exception();

		print_or_store( parsed_first_line , headers, raw_body );
	}
	catch(const std::exception& e) {

		std::cout << "error 400, bad request" << std::endl;
	}

	return (0);
}