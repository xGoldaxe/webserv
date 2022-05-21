#include "../webserv.hpp"

int	http_header_content_type( Request &req, Response &res ) {

	(void) req;
	res.add_header("Content-Type", "text/" + get_extension( req.getUrl() ) );
	return (1);
}