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
/* fill data from request */

Route find_route(std::vector<Route> routes, std::string url);

// change it through the config
#define MAX_BUFFER_SIZE 16384
#define TIMEOUT_TIME 3
void	Request::try_construct( std::string raw_request, Webserv_conf conf) 
{
	this->conf = conf;
	store_req(true, this);

	preq::parse_request( raw_request, &(store_data_from_raw_req) );

	this->route = find_route(conf.getServers()[0].getRoutes(), this->legacy_url);
	
	this->auto_index = false;
	
	/* find body_length */
	std::map<std::string, std::string> ::iterator it = this->headers.find( "Content-Length" );
	if ( it != this->headers.end() )
	{
		char	*end_ptr;
		this->body_length = strtoul( (it->second).c_str(), &end_ptr, 10 );
	}
	else
		this->body_length = 0;
	/* find body length */

	this->request_validity = true;
};

Route find_route(std::vector<Route> routes, std::string url)
{

	Route route;
	for (std::vector<Route>::iterator it = routes.begin(); it != routes.end(); ++it)
	{
		std::string test_url = finish_by_only_one(url, '/');
		if (strncmp(test_url.c_str(), it->get_location().c_str(), it->get_location().size() - 1) == 0)
		{
			route = Route(*it, 1);
		}
	}

	return route;
}