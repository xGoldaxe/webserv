#include "webserv.hpp"

std::string to_string( int number ) {

	std::string result;

	std::ostringstream convert;
	convert << number;  
	result = convert.str();
	return ( result );
}