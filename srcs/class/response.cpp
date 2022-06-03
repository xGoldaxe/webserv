#include "../webserv.hpp"
#include "response.hpp"
#include <sys/socket.h>
#include <dirent.h> 
#include <stdio.h>

Response::Response( int client_socket, webserv_conf &conf, Request const &req ) : conf(conf), req( req ) {
	
	this->client_socket = client_socket;
	this->version = this->conf.http_version;
	this->status_code = -1;

	if ( req.is_request_valid() == false )
		this->set_status( 400, "Bad Request" );
	else if ( this->version != req.get_http_version() )
		this->set_status( 505, "HTTP Version Not Supported" );
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

	/* add some headers */
	http_header_content_length( this->req, *this );

	std::string raw_response;

	raw_response = this->version + " " + to_string( this->status_code ) + " " + this->status_message;
	for ( headers_t::iterator it = this->headers.begin(); it != this->headers.end(); ++it )
		raw_response += "\n" + it->first + ": " + it->second;
	raw_response += "\r\n\n";
	raw_response += body;


	#ifdef PRINT_REQ
		std::cout << "<====>" << std::endl;
		std::cout << raw_response << std::endl;
		std::cout << "<====>" << std::endl;
	#endif

	int status = ::send(this->client_socket, raw_response.c_str(), raw_response.size(), 0);


	return (status);
}

std::string auto_index_template( std::string url, std::string legacy_url );

/**
 * @todo free all string_to_char
 */
std::string Response::load_body( Request &req )
{
	std::string new_body;
	if (false /* req.auto_index == true */ ) {
		this->add_header("Content-Type", "text/html");
		new_body = auto_index_template( req.getUrl(), req.get_legacy_url() );
	}
	else if (req.get_route().cgi_enable == true  && get_extension( req.getUrl().c_str() ) == req.get_route().cgi_extension) {
		std::vector<MimeType> cgi_mime_types;
		cgi_mime_types.push_back(MimeType("application", "php", "php", true));
		CGIManager cgi(cgi_mime_types, "/usr/bin/php-cgi");
		new_body = cgi.exec(req);
	}
	else
	{
		new_body = read_binary( req.getUrl() );
	}
	this->body = new_body;
	return this->body;
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

std::string & Response::error_body(void) {

	this->add_header("Content-Type", "text/html");
	try
	{
		//this line throw an error if page not find
		std::string filename = this->req.route.error_pages.at( this->status_code );
		if ( usable_file( filename ) )
			this->body = read_binary( filename );
		else
		{
			this->set_status( 500, "Internal Server Error" );
			this->body = error_template(this->get_str_code(), this->status_message);
		}
	}
	catch(const std::exception& e)
	{
		this->body = error_template(this->get_str_code(), this->status_message);
	}
	
	return (this->body);
}

std::string auto_index_template( std::string url, std::string legacy_url ) {

	std::string files;
	std::cout << "auto_index_template" << std::endl;
	/* get each files */
	DIR *d;
	struct dirent *dir;
	d = opendir( url.c_str() );
	if (d) {
		while ((dir = readdir(d)) != NULL) {
			std::string dirname = dir->d_name;
			if ( is_file( std::string( finish_by_only_one( url, '/' ) + dirname ).c_str() ) == 0 )
				dirname = finish_by_only_one( dirname, '/' );
			if ( dirname != "./" )
				files += "<p><a href=\"" + finish_by_only_one( legacy_url, '/' ) + dirname + "\">" + dirname + "</a></p>";
		}
		closedir(d);
	}
	return std::string( std::string("<!DOCTYPE html>\
		<html lang=\"en\">\
		<head>\
			<meta charset=\"UTF-8\">\
			<meta http-equiv=\"X-UA-Compatible\" content=\"IE=edge\">\
			<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\
			<title>Index of ")
			+ legacy_url
			+ std::string ("</title>\
			\
			<style>\
			p, h1 {\
				text-align: center;\
			}\
			</style>\
		</head>\
		<body>\
			<h1>Index of ")
		+ legacy_url
		+ std::string( "</h1>\
			<hr />\
			<p>")
		+ files
		+ std::string("</p>\
		<hr />\
		</body>\
		</html>"));
	}