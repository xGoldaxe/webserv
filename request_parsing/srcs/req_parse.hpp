#pragma once

#include <unistd.h>
#include <iostream>
#include <string.h>
#include <vector>
#include <map>
#include <sstream>
#include <iomanip>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdlib.h>
#include "../../srcs/errors/http_code.hpp"

#define MAX_URL_SIZE 5000

void	print_parsed_req( 
	std::vector<std::string> parsed_first_line,
	std::map<std::string, std::string> headers,
	std::string path_info
);

namespace preq {
	bool	verify_absolute_url( std::string url );
	bool	check_line( std::string str );
	bool	check_and_trunc_line( std::string *line );
	bool	is_space( char c );
	std::vector<std::string>	read_until( const std::string & data, bool (*rule)(std::string) );
	std::vector<std::string>	split_str( std::string str );
	std::vector<std::string>	split_str_c( const std::string & str, char c );
	std::string	left_trim( std::string str, bool (*rule)(char) );
	std::string	right_trim( std::string str, bool (*rule)(char) );
	std::string	trim( std::string str, bool (*rule)(char) );
	std::map<std::string, std::string>	get_headers_req( std::vector<std::string>::iterator begin,
			std::vector<std::string>::iterator end );
	bool	parse_url( std::string url, std::string & ret_url, std::string & query_string );
	std::string replace_percent( std::string url );
	bool	is_full_hexa( std::string s );
	size_t	check_str_size_memory( bool mode, size_t value = 0 );
	bool	check_str_size( std::string str );
	std::vector<std::string>	parse_start_line( std::string raw_line );
	int parse_request( std::string & data, void (*print_or_store)(std::vector<std::string>,
		std::map<std::string, std::string>, std::string ) );
}