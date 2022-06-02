#include "req_parse.hpp"
#include "parse_request.hpp"

int main( void ) {

	try {
		preq::parse_request( STDIN_FILENO, &print_parsed_req );
	}
	catch(const std::exception& e) {
		std::cout << "error 400, bad request" << std::endl;
	}

	return (0);
}