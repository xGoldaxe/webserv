#include "../webserv.hpp"
#include <sys/types.h>
#include <sys/socket.h>
#include <cstdlib>
#include "unistd.h"

Response::Response( int client_socket, webserv_conf &conf ) : conf(conf) {
	
	this->client_socket = client_socket;
	version = this->conf.http_version;
}

Response::~Response( void ) {}

std::string Response::get_str_code(void) {

	return to_string( this->status_code );
}

int	Response::add_header( std::string key, std::string value ) {

	this->headers.insert( headers_t::value_type(key, value) );
	return (1);
}


void	Response::set_status( int status_code, std::string msg ) {
	this->status_code = status_code;
	this->status_message = msg;
}

int	Response::send() {

	std::string raw_response;

	raw_response = this->version + " " + to_string( this->status_code ) + this->status_message;
	for ( headers_t::iterator it = this->headers.begin(); it != this->headers.end(); ++it )
		raw_response += "\n" + it->first + ": " + it->second;
	raw_response += "\r\n\n";
	raw_response += body;

	int status = ::send(this->client_socket, raw_response.c_str(), raw_response.size(), 0);
	return (status);
}

std::string Response::load_body( Request &req ) {

	if ( req.get_route()->cgi_enable == true 
		&& get_extension( req.getUrl().c_str() ) == req.get_route()->cgi_extension ) {

		std::cout << std::string( req.get_route()->cgi_path + " " + req.getUrl() ) << std::endl;
		std::system( std::string( req.get_route()->cgi_path + " " + req.getUrl() ).c_str() );
	}
	else
		this->body = read_binary( req.getUrl() );
	return this->body;
}

std::string error_template(std::string error_code, std::string message);

std::string & Response::error_body(void) {

	this->body = error_template(this->get_str_code(), this->status_message);
	return (this->body);
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