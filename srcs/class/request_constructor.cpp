#include "webserv.hpp"
#include <string.h>
#include <unistd.h>
#include "../../request_parsing/srcs/req_parse.hpp"
#include "../../request_parsing/srcs/parse_request.hpp"

Request *store_req(bool mode, Request *req = NULL)
{

	static Request *stored_req;

	if (mode == true)
		stored_req = req;
	return (stored_req);
}

void store_data_from_raw_req(
	std::vector<std::string> parsed_first_line,
	std::map<std::string, std::string> headers,
	std::string raw_body)
{
	Request *stored_req = store_req(false);

	if (!stored_req)
		throw std::exception();

	stored_req->fill_start_line(
		parsed_first_line[0],
		parsed_first_line[1],
		parsed_first_line[2]);
	stored_req->fill_headers(headers);
	stored_req->fill_body(raw_body);
}

/* fill data from request */
void Request::fill_start_line(std::string method, std::string url, std::string version)
{

	this->method = method;
	this->legacy_url = url;
	this->version = version;
}

void Request::fill_headers(std::map<std::string, std::string> headers)
{

	this->headers = headers;
}

void Request::fill_body(std::string body)
{

	this->body = body;
}

// change it through the config
#define MAX_BUFFER_SIZE 16384
#define TIMEOUT_TIME 3
void	Request::try_construct( std::string raw_request, Webserv_conf conf) 
{
	try
	{
		this->conf = conf;
		store_req(true, this);

		preq::parse_request( raw_request, &(store_data_from_raw_req) );

		this->route = find_route(conf.getServers()[0].getRoutes(), this->legacy_url);
		
		this->auto_index = false;
		
		/* find body_length */
		std::map<std::string, std::string> ::iterator t_encoding = this->headers.find( "Transfer-Encoding" );
		std::map<std::string, std::string> ::iterator c_length = this->headers.find( "Content-Length" );
		if ( t_encoding != this->headers.end() )
		{
			if ( c_length != this->headers.end() )
				throw std::exception();
			if ( t_encoding->second != std::string( "chunked" ) )
				throw std::exception();
			this->body_transfer = CHUNKED;
		}

		if ( c_length != this->headers.end() )
		{
			char	*end_ptr;
			this->body_length = strtoul( (c_length->second).c_str(), &end_ptr, 10 );
			this->body_transfer = LENGTH;
		}

		if ( c_length == this->headers.end() && t_encoding == this->headers.end() )
		{
			this->fulfilled = true;
			this->body_transfer = NO_BODY;
		}

		this->request_validity = true;
	}
	catch(const std::exception& e)
	{
		this->set_status( 400, "Bad Request" );
	}
};
