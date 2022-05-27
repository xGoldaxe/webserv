#include "../webserv.hpp"

int	http_header_server( Request &req, Response &res ) {
	//edited to use first server name
	(void) req;
	res.add_header( "Server", res.get_conf().servers[0].getName() );
	return (1);
}