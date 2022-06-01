#include "../webserv.hpp"
#include <algorithm>
#include <string.h>

Route find_route( std::vector<Route> routes, std::string url ) {

	Route route;
	for ( std::vector<Route>::iterator it = routes.begin(); it != routes.end(); ++it )
	{
		std::string test_url = finish_by_only_one( url, '/' );
		if (strncmp(test_url.c_str(), it->get_location().c_str(), it->get_location().size() - 1) == 0)
		{
			route = Route(*it, 1);
		}
	}
	// std::cout << "const " << (*route)->error_pages.begin()->second << std::endl;

	return route;
}

Request::Request( std::string raw_data, Webserv_conf &conf ) : conf(conf) {

	std::string line = raw_data.substr( 0, raw_data.find("\r") );
	std::vector<std::string> splitted_str = split_str( line );

	this->route = conf.servers[0].getRoutes()[0]; // using the default fallback route
	
	/* need further verifications */
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
		
	// this->url = find_route( &this->route, conf.routes, this->legacy_url );
	this->route = find_route( conf.servers[0].getRoutes(), this->legacy_url );

	this->url = this->route.get_root()
				+ this->legacy_url.substr( this->legacy_url.find_first_of( this->route.get_location() ) + this->route.get_location().size() );
	this->url = this->url.substr( 0, this->url.size() );

	std::cout << this->url << std::endl;

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
	return (this->conf.servers[0].getRoot() + url);
}
std::string Request::get_http_version(void) const {
	return this->version;
}
Route	Request::get_route(void) {
	return (this->route);
}

bool	Request::is_request_valid(void) const {
	return request_validity;
}

bool	Request::is_allowed_method( const std::string &method ) const {

	return ( std::find( this->route.get_methods().begin(), this->route.get_methods().end(), method) != this->route.get_methods().end() );
} 

std::string Request::try_url( Response & res ) {

	res.set_status( 404, "Not Found" ); // fallback if 0 condition has been checked

	try
	{
		std::string	redir = this->route.get_redirections().at( 
			this->get_legacy_url() );

		res.set_status( 301, "Moved Permanently" );
		res.add_header( "Location", redir );
		return ( this->url );
	}
	catch( const std::exception& e ) {}

	if ( this->route.get_auto_index() && is_file( this->url.c_str() ) == 0 ) // this is a folder
	{
		res.set_status( 200, "OK" );
		this->auto_index = true;
	}
	else if ( file_exist( this->url ) && is_file( this->url.c_str() ) == 1 ) // the file exist
	{
		if ( file_readable( this->url ) )
			res.set_status( 200, "OK" );
		else
			res.set_status( 403, "Forbidden" );
	}
	else
	{

		std::string test_url = finish_by_only_one( this->url, '/' );
		std::vector<std::string> tempPtr = this->conf.servers[0].getIndex();
		for ( std::vector<std::string>::iterator it = tempPtr.begin();
			it != tempPtr.end() ; ++it )
		{
			test_url += *it;

			if ( file_exist(test_url) && is_file( test_url.c_str() ) == 1 )
			{
				res.set_status( 200, "OK" );
				this->url = test_url;
			}
		}
	}
	return ( this->url );
}