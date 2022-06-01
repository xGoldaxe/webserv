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

namespace preq {
	
	int parse_request( int sock_fd );
void	print_or_store ( 
						std::vector<std::string> parsed_first_line,
						std::map<std::string, std::string> headers,
						std::string raw_body
					);
std::string	read_until( int fd, bool (*rule)(std::string) );
std::vector<std::string>	split_str( std::string str );
std::string	left_trim( std::string str, bool (*rule)(char) );
std::string	right_trim( std::string str, bool (*rule)(char) );
std::string	trim( std::string str, bool (*rule)(char) );
}
