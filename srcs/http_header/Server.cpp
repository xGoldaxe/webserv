#include "../webserv.hpp"

int	http_header_server( Request &req, Response &res )
{
	(void) req;
	res.add_header( "Server", WEBSERV_VERSION);
	return (1);
}