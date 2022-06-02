#include "../webserv.hpp"
#include <string.h>
#include <unistd.h>
#include "../../request_parsing/srcs/req_parse.hpp"
#include "../../request_parsing/srcs/parse_request.hpp"

Request	*store_req( bool mode, Request * req = NULL ) {

	static Request *stored_req;

	if ( mode == true )
		stored_req = req;
	return (stored_req);
}

void	store_data_from_raw_req( 
	std::vector<std::string> parsed_first_line,
	std::map<std::string, std::string> headers,
	std::string	raw_body
)
{
	Request *stored_req = store_req( false );

	if ( !stored_req )
		throw std::exception();

	stored_req->fill_start_line(
		parsed_first_line[0],
		parsed_first_line[1],
		parsed_first_line[2]
	);
	stored_req->fill_headers( headers );
	stored_req->fill_body( raw_body );
}

/* fill data from request */
void	Request::fill_start_line( std::string method, std::string url, std::string version ) {

	this->method = method;
	this->legacy_url = url;
	this->version = version;
}

void	Request::fill_headers( std::map<std::string, std::string> headers ) {

	this->headers = headers;
}

void	Request::fill_body( std::string body ) {

	this->body = body;
}
/* fill data from request */

Route find_route( std::vector<Route> routes, std::string url );

Request::Request( int socket_data, webserv_conf &conf ) : conf(conf) {

	store_req( true, this );
	preq::parse_request( socket_data, &( store_data_from_raw_req ) );

	//file informations
	request_validity = true;
	this->route = find_route( conf.routes, this->legacy_url );

	this->url = this->route.root
				+ this->legacy_url.substr( this->legacy_url.find_first_of( this->route.location ) + this->route.location.size() );
	this->url = this->url.substr( 0, this->url.size() );

	this->auto_index = false;
};

Route find_route( std::vector<Route> routes, std::string url ) {

	Route route;
	for ( std::vector<Route>::iterator it = routes.begin(); it != routes.end(); ++it )
	{
		std::string test_url = finish_by_only_one( url, '/' );
		if ( strncmp( test_url.c_str(), it->location.c_str(), it->location.size() - 1 ) == 0 )
		{
			route = *it;
		}
	}
	// std::cout << "const " << (*route)->error_pages.begin()->second << std::endl;

	return route;
}