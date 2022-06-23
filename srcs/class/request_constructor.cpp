#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <string.h>
#include <unistd.h>
#include "request.hpp"
#include "../../request_parsing/srcs/req_parse.hpp"

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
	std::string query_string
)
{
	Request *stored_req = store_req(false);

	if (!stored_req)
		throw HTTPCode500();

	stored_req->fill_start_line(
		parsed_first_line[0],
		parsed_first_line[1],
		parsed_first_line[2]);
	stored_req->fill_query( query_string );
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
void	Request::try_construct( std::string raw_request, Bundle_server bundle) 
{
	try
	{
		store_req(true, this);

		preq::parse_request( raw_request, &(store_data_from_raw_req) );

		std::string host = this->get_header_value("Host");
		if (host == "") {
			throw HTTPCode400();
		}
		if (host.find_first_of(':') != host.npos) {
			host = std::string(host, 0, host.find_first_of(':'));
		}

		this->route = find_route(bundle.get_server_from_server_name(host).getRoutes(), this->legacy_url, this->method);
		this->auto_index = this->route.get_auto_index();
		
		/* find body_length */
		for ( std::map<std::string, std::string>::iterator it = this->headers.begin(); it != this->headers.end(); ++it )
		{
			if ( it->first == "Transfer-Encoding" )
				this->transfer_encoding( it->second );
			else if ( it->first == "Content-Length" )
				this->content_length( it->second );
			else if ( it->first == "Content-Type" )
				this->multipart( it->second );
		}
		
		if ( this->fulfilled == false )
			this->state = FEEDING;
		else
			this->state = READY;

		this->request_validity = true;
	}
	catch(const HTTPError& e)
	{
		this->request_validity = false;
		this->set_status( e.getCode(), e.getDescription() );
	}
};
