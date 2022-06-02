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

void	print_parsed_req( 
	std::vector<std::string> parsed_first_line,
	std::map<std::string, std::string> headers,
	std::string	raw_body
);