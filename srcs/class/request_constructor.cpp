#include "../webserv.hpp"
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

	std::cout << parsed_first_line[0] << std::endl;
	std::cout << parsed_first_line[1] << std::endl;
	std::cout << parsed_first_line[2] << std::endl;
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
Request::Request( std::string raw_request, Webserv_conf conf) : conf(conf)
{
	store_req(true, this);

	preq::parse_request( raw_request, &(store_data_from_raw_req) );


	// file informations
	request_validity = true;
	this->route = find_route(conf.getServers()[0].getRoutes(), this->legacy_url);


	this->url = this->route.get_root() + this->legacy_url.substr(this->legacy_url.find_first_of(this->route.get_location()) + this->route.get_location().size());
	this->url = this->url.substr(0, this->url.size());
	

	this->auto_index = false;
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
	// std::cout << "const " << (*route)->error_pages.begin()->second << std::endl;

	return route;
}