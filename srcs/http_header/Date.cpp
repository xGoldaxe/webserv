#include "../webserv.hpp"
#include <stdio.h>
#include <time.h>  

int	http_header_date( Request &req, Response &res ) {

	(void) req;
	res.add_header("Date", "Wed, 21 Oct 2015 07:28:00 GMT");
	return (1);
}