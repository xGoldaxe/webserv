#include <iostream>
#include <fstream>
#include <string>
#include <string.h>
#include <vector>
#include <sstream>
#include <filesystem>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "webserv.hpp"

int http_GET( Request &req, Response &res ) {
	
	req.try_url( res );
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

int http_get_response( Request &req, Response &res ) {

	if ( res.status_code >= 400 ) /* if an error occure before executing the requst e.g invalid request */
	{
		res.error_body();
		res.send();
		close( res.client_socket );
		return 1;
	}
	else
	{
		if ( verify_method( req, "GET" ) )
			http_GET(req, res);
		else if ( verify_method( req, "POST" ) )
			http_GET(req, res);
		else if ( verify_method( req, "DELETE" ) )
			http_GET(req, res);
		else
		{
			res.set_status( 405, "Method Not Allowed" );
			res.error_body();
		}
		/* generic headers */
		http_header_date(req, res);
		http_header_server(req, res);
		res.add_header( "Connection", "keep-alive" );

		res.send();
	}
	return 1;
}
