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

std::string read_binary( std::string filename ) {

	std::ifstream input( filename.c_str(), std::ios::binary );
    std::istreambuf_iterator<char> it(input), end;
    std::string ss(it, end);
	return ss;
}

std::string get_extension(std::string file_name)
{
	int position=file_name.find_last_of(".");
	return file_name.substr(position+1);
}

int http_GET( Request &req, Response &res ) {
	
	std::string url = req.tryUrl( &(res.status_code), &(res.status_message) );
	if ( res.status_code == 200 )
	{
		res.body = read_binary( url );
		/* get content type */
		res.add_header("Content-Type", "text/" + get_extension( url ) );
		/* get content type */
		return (res.status_code);
	}
	res.error_body();
	return (res.status_code);
}

int http_get_response( Request &req, Response &res )
{
	if (strcmp(req.getMethod().c_str(), "GET") == 0)
		http_GET(req, res);
	else
	{
		res.set_status( 405, "Method Not Allowed" );
		res.error_body();
	}
	res.add_header( "Content-Length", to_string( res.body.size() ) );
	res.add_header( "Server", res.get_conf().server_name );
	res.send();
	return 1;
}