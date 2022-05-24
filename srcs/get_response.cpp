#include <iostream>
#include <fstream>
#include <string>
#include <string.h>
#include <vector>
#include <sstream>
#include <filesystem>
#include <stdio.h>
#include <stdlib.h>

#include "webserv.hpp"

int http_GET( Request &req, Response &res ) {
	
	req.try_url( &(res.status_code), &(res.status_message) );
	if ( res.status_code == 200 )
	{
		res.load_body( req );
		http_header_content_type(req, res);
	}
	else
	{
		res.error_body();
	}
	return (res.status_code);
}

bool verify_method( const Request &req, std::string method ) {
	return strcmp( req.getMethod().c_str(), method.c_str() ) == 0;
	return strcmp( req.getMethod().c_str(), method.c_str() ) == 0 && req.is_allowed_method(method);
}

int http_get_response( Request &req, Response &res )
{
	if ( verify_method( req, "GET" ) )
		http_GET(req, res);
	else
	{
		res.set_status( 405, "Method Not Allowed" );
		res.error_body();
	}
	/* generic headers */
	http_header_content_length(req, res);
	http_header_date(req, res);
	http_header_server(req, res);
	res.add_header( "Connection", "keep-alive" );

	res.send();
	return 1;
}