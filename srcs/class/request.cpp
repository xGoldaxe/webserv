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

Request::Request( std::string raw_data, webserv_conf &conf ) : conf(conf) {

	std::string line = raw_data.substr( 0, raw_data.find("\r") );
	std::vector<std::string> splitted_str = split_str( line );

	this->route = &(conf.routes[0]); // using the default fallback route

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

	this->url = find_route( &this->route, conf.routes, this->legacy_url );
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
	return (this->conf.root + url);
}
std::string Request::get_http_version(void) const {
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

std::string Request::try_url( Response & res ) {

	res.set_status( 404, "Not Found" ); // fallback if 0 condition has been checked

	try
	{
		std::string	redir = this->route->redirections.at( 
			this->get_legacy_url() );

		res.set_status( 301, "Moved Permanently" );
		res.add_header( "Location", redir );
		return ( this->url );
	}
	catch(const std::exception& e) {
		std::cout << "catched" << std::endl;
	}

	if ( this->route->auto_index && is_file( this->url.c_str() ) == 0 ) // this is a folder
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

		for ( std::vector<std::string>::iterator it = this->conf.index.begin();
			it != this->conf.index.end() ; ++it )
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