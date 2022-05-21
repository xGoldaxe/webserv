#include "../webserv.hpp"

int	http_header_server( Request &req, Response &res ) {

	(void) req;
	res.add_header( "Server", res.get_conf().server_name );
	return (1);
}