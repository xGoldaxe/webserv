#include "req_parse.hpp"
#include <algorithm>
#include "../../srcs/errors/http_code.hpp"

size_t	preq::check_str_size_memory( bool mode, size_t value ) {

	static size_t	stored_value = 0;
	if ( mode == true )
		stored_value = value;
	return stored_value;
}

bool	preq::check_str_size( std::string str ) {

	return ( str.size() >= check_str_size_memory( false ) );
}

std::vector<std::string>	preq::parse_start_line( std::string raw_line ) {

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

/* during the parsing at any time, an exception could happened. We just have to
catch it and send back an error 400 response */

int preq::parse_request(
std::string & data,
void (*print_or_store)(std::vector<std::string>, std::map<std::string, std::string>, std::string )
) {

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
	if ( preq::parse_url( parsed_first_line[1], parsed_first_line[1], query_string ) == false )
		throw HTTPCode400();

	if ( preq::verify_absolute_url( parsed_first_line[1] ) == false )
		throw HTTPCode400();

	if ( parsed_first_line[2] != "HTTP/1.1")
		throw HTTPCode505();
	
	print_or_store( parsed_first_line, headers, query_string );
	return (0);
}