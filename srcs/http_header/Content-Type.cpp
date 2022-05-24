#include "../webserv.hpp"

int	http_header_content_type( Request &req, Response &res ) {

	(void) req;
	if ( get_extension( req.getUrl() ) == "jpg" )
		res.add_header("Content-Type", "image/jpeg");
	else
		res.add_header("Content-Type", "text/" + get_extension( req.getUrl() ) );
	return (1);
}