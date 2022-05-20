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

std::string error_template(std::string error_code, std::string message) {

	return std::string( std::string("<!DOCTYPE html>\
	<html lang=\"en\">\
	<head>\
		<meta charset=\"UTF-8\">\
		<meta http-equiv=\"X-UA-Compatible\" content=\"IE=edge\">\
		<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\
		<title>Webserv - ")
		+ error_code
		+ std::string ("</title>\
		\
		<style>\
		p, h1 {\
			text-align: center;\
		}\
		</style>\
	</head>\
	<body>\
		<h1>WEBSERV ERROR - ")
	+ error_code
	+ std::string( "</h1>\
		<hr />\
		<p>")
	+ message
	+ std::string("</p>\
	</body>\
	</html>"));
}

void http_GET( Request &req, std::string &res ) {
	
	int				status;

	std::string url = req.tryUrl(&status);
	if ( status == 200 )
	{
		res = "HTTP/1.1 200 OK\r\n\n";
		res += read_binary( url );
	}
	else
	{
		res = "HTTP/1.1 404 Not Found\r\n\n";
		res += error_template(std::string( "404"), std::string("Not found") );
	}
}

int http_get_response( Request &req, std::string &res )
{
	if (strcmp(req.getMethod().c_str(), "GET") == 0)
		http_GET(req, res);
	return 1;
}