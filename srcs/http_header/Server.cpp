#include "../webserv.hpp"

int	http_header_server( Request &req, Response &res ) {

	(void) req;
	if (!res.get_conf().servers.empty())
	{
		res.add_header( "Server", res.get_conf().servers[0].getName() );	
	}else
	{
		res.add_header( "Server", "default");
	}
	return (1);
}