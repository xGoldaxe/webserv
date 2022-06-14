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
	std::map<std::string, std::string> headers)
{
	Request *stored_req = store_req(false);

	if (!stored_req)
		throw HTTPCode500();

	if (parsed_first_line[1].find_first_of('?') != std::string::npos) {
		std::string filename = parsed_first_line[1].substr(0, parsed_first_line[1].find_first_of('?'));
		std::string query = parsed_first_line[1].substr(parsed_first_line[1].find_first_of('?') + 1);

		stored_req->fill_start_line(
			parsed_first_line[0],
			filename,
			parsed_first_line[2]);
		stored_req->fill_query(query);
	} else {
		stored_req->fill_start_line(
			parsed_first_line[0],
			parsed_first_line[1].substr(0, parsed_first_line[1].find_first_of('?')),
			parsed_first_line[2]);
	}
	stored_req->fill_headers(headers);
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

void Request::fill_query(std::string query)
{
	this->query = query;
}

void Request::fill_body(std::string body)
{

	this->body = body;
}

// change it through the config
#define MAX_BUFFER_SIZE 16384
#define TIMEOUT_TIME 3
void	Request::try_construct( std::string raw_request, std::vector<Route> routes) 
{
	try
	{
		store_req(true, this);

		preq::parse_request( raw_request, &(store_data_from_raw_req) );

		this->route = find_route(routes, this->legacy_url);
		this->auto_index = this->route.get_auto_index();
		
		/* find body_length */
		std::map<std::string, std::string> ::iterator t_encoding = this->headers.find( "Transfer-Encoding" );
		std::map<std::string, std::string> ::iterator c_length = this->headers.find( "Content-Length" );
		if ( t_encoding != this->headers.end() )
		{
			if ( c_length != this->headers.end() )
				throw std::exception();
			this->set_status( 501, "Not Implemented" );
			return ;
		}

		if ( c_length != this->headers.end() )
		{
			char	*end_ptr;
			this->body_length = strtoul( (c_length->second).c_str(), &end_ptr, 10 );
			this->body_transfer = LENGTH;
			if ( this->body_length == 0 )
				this->fulfilled = true;
		}
		else
		{
			this->fulfilled = true;
			this->body_transfer = NO_BODY;
		}

		this->request_validity = true;
	}
	catch(const HTTPError& e)
	{
		this->set_status( e.getCode(), e.getDescription() );
	}
};
