#include "req_parse.hpp"
#include <algorithm>
#include "utils.hpp"
#include "parse_url.hpp"
#include "verify_absolute_url.hpp"
#include "../../srcs/errors/http_code.hpp"

namespace	preq {

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

		if ( raw_line.size() > 0 && (raw_line.at(0) == ' ' || raw_line.at( raw_line.size() - 1 ) == ' ') )
			throw HTTPCode400();
		std::vector<std::string> splited = split_str( raw_line );
		if ( splited.size() != 3 )
			throw HTTPCode400();
		for ( int i = 0; i < 3; ++i )
		{
			if ( splited[i].size() < 1 )
				throw HTTPCode400();
		}
		return splited;
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

	/* during the parsing at any time, an exception could happened. We just have to
	catch it and send back an error 400 response */

	int parse_request( std::string & data, void (*print_or_store)(std::vector<std::string>, std::map<std::string, std::string>, std::string, std::string) ) {

		std::vector<std::string> lines = read_until( data, &check_line );
		if ( lines.size() < 1 )
			throw HTTPCode400();

		// first line
		std::string start_line = lines[0];
		if ( check_and_trunc_line( &start_line ) == false )
			throw HTTPCode400();
		std::vector<std::string> parsed_first_line = parse_start_line( start_line );
		

		// headers
		std::map<std::string, std::string> headers;
		if ( lines.size() > 1 )
			headers = get_headers_req( lines.begin() + 1, lines.end() );


		//verify data
		std::vector<std::string> a_meth;
		a_meth.push_back( "HEAD" );
		a_meth.push_back( "GET" );
		a_meth.push_back( "POST" );
		a_meth.push_back( "DELETE" );
		if ( std::find( a_meth.begin(), a_meth.end(), parsed_first_line[0] ) == a_meth.end() )
			throw HTTPCode501();
		

		std::string query_string;
		std::string path_info;
		if ( parse_url( parsed_first_line[1], parsed_first_line[1], path_info, query_string ) == false )
			throw HTTPCode400();

		if ( verify_absolute_url( parsed_first_line[1] ) == false )
			throw HTTPCode400();

		if ( parsed_first_line[2] != "HTTP/1.1")
			throw HTTPCode505();
		
		print_or_store( parsed_first_line, headers, query_string, path_info );
		return (0);
	}
}