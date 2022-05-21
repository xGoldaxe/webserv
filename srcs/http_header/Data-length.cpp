#include "../webserv.hpp"

int	http_header_content_length( Request &req, Response &res ) {

	(void) req;
	res.add_header( "Content-Length", to_string( res.body.size() ) + " bytes");
	return (1);
}