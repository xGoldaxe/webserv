#include "req_parse.hpp"

void	print_parsed_req( 
	std::vector<std::string> parsed_first_line,
	std::map<std::string, std::string> headers,
	std::string query_string,
	std::string path_info
) {

	// first_line
	std::cout << "METHOD: " << parsed_first_line[0] << std::endl;
	std::cout << "PATH: " << parsed_first_line[1] << std::endl;
	std::cout << "VERSION: " << parsed_first_line[2] << std::endl;
	
	// headers
	for ( std::map<std::string, std::string>::iterator it = headers.begin(); it != headers.end(); ++it )
	{
		std::cout << "==>[" << it->first << "]" << ": [" << it->second << "]" << std::endl;
	}

	std::cout << query_string;
	std::cout << path_info;
}