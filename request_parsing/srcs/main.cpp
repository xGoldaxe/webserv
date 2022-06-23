#include "req_parse.hpp"

#define MAX_BUFFER_SIZE 16384 /** @ignore */
int main( void ) {

	char data[MAX_BUFFER_SIZE];
	try {
		bzero(data, MAX_BUFFER_SIZE);
		read( STDIN_FILENO, data, MAX_BUFFER_SIZE - 1 ); /** @xGoldaxe on peut supprimer ce fichier non?, autant garder les testes @restray */
		std::string str_data( data );
		preq::parse_request( str_data, &print_parsed_req );
	}
	catch(const HTTPError& e)
	{
		std::cout << "error " << e.getCode() << ", " << e.getDescription() << std::endl;
	}

	return (0);
}