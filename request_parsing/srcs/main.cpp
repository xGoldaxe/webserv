#include "req_parse.hpp"

int main( void ) {

	try {
		preq::parse_request( STDIN_FILENO );
	}
	catch(const std::exception& e) {
		std::cout << "error 400, bad request" << std::endl;
	}

	return (0);
}