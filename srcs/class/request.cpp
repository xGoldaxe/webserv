#include "../webserv.hpp"
#include <algorithm>
#include <string.h>

std::string find_route( Route **route, std::vector<Route> routes, std::string url ) {

	std::string tmp_url;
	for ( std::vector<Route>::iterator it = routes.begin(); it != routes.end(); ++it )
	{
		std::string test_url = finish_by_only_one( url, '/' );
		if ( strncmp( test_url.c_str(), it->location.c_str(), it->location.size() - 1 ) == 0 )
		{
			*route = &(*it);
			tmp_url = (*route)->root
				+ test_url.substr( test_url.find_first_of( it->location ) + it->location.size() );
			tmp_url = tmp_url.substr(0, tmp_url.size()-1);
		}
	}
	return tmp_url;
}

Request::Request( std::string raw_data, Webserv_conf &conf ) : conf(conf) {

	std::string line = raw_data.substr( 0, raw_data.find("\n") );
	std::vector<std::string> splitted_str = split_str( line );
	
	if ( splitted_str.size() != 3 )
	{
		request_validity = false;
		return ;
	}
	//file informations
	request_validity = true;
	this->method = splitted_str[0];
	this->legacy_url = splitted_str[1];
	this->version = splitted_str[2];
	//Edited v to take routes from first server, to edit for multi-server
	this->url = find_route( &this->route, conf.servers[0].getRoutes(), this->legacy_url );
	this->row_data = raw_data;
	this->auto_index = false;
};

Request::~Request( void ) {};

/* end coplien */
std::string	Request::getMethod(void) const{
	return (method);
}
std::string Request::getBody(void) {
	return (body);
}
std::string Request::getUrl(void) {
	return (url);
}
std::string Request::get_legacy_url(void) {
	return this->legacy_url;
}
std::string Request::getRelativeUrl(void) {
	std::cout << "out of date" << std::endl;
//Edited v to take routes from first server, to edit for multi-server
	return (this->conf.servers[0].getRoot() + url);
}
std::string Request::get_http_version(void) const {
	std::cout << this->version << " size:" << this->version.size() << std::endl;
	return this->version;
}
Route	*Request::get_route(void) {
	return (this->route);
}

bool	Request::is_request_valid(void) const {
	return request_validity;
}

bool	Request::is_allowed_method( const std::string &method ) const {

	return ( std::find( this->route->methods.begin(), this->route->methods.end(), method) != this->route->methods.end() );
}

std::string Request::try_url( int *status, std::string *message ) {

	*status = 404;
	*message = "Not Found";
	if ( this->route->auto_index && is_file( this->url.c_str() ) == 0 )
	{
		*status = 200;
		*message = "OK";
		this->auto_index = true;
	}
	else if ( file_exist( this->url ) && is_file( this->url.c_str() ) == 1 )
	{
		if ( file_readable( this->url ) )
		{
			*status = 200;
			*message = "OK";
		}
		else
		{
			*status = 403;
			*message = "Forbidden";
		}
	}
	else
	{

		std::string test_url = finish_by_only_one( this->url, '/' );
	//Had to edit v to take routes from first server, to edit for multi-server
	//tempPtr is to prevent dangling temporary variable
		std::vector<std::string> tempPtr = this->conf.servers[0].getIndex();
		for ( std::vector<std::string>::iterator it = tempPtr.begin();
			it != this->conf.servers[0].getIndex().end() ; ++it )
		{
			test_url += *it;

			// std::cout << " try... "  << test_url << ":" << this->url << std::endl;

			if ( file_exist(test_url) && is_file( test_url.c_str() ) == 1 )
			{
				*status = 200;
				*message = "OK";
				this->url = test_url;
			}
		}
	}
	return ( this->url );
}