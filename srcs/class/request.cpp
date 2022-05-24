#include "../webserv.hpp"
#include <algorithm>
#include <string.h>

Request::Request( std::string raw_data, webserv_conf &conf ) : conf(conf) {

	std::vector<std::string> splitted_str;
	// split the first line
	std::string first_line = raw_data.substr(0, raw_data.find("\n"));
	std::stringstream stream_str(first_line);
	std::string buffer_str;
	while (std::getline(stream_str, buffer_str, ' '))
		splitted_str.push_back(buffer_str);
	
	//file informations
	this->method = splitted_str.at(0);
	/* find route */
	this->url = splitted_str.at(1);
	this->legacy_url = this->url;
	std::string tmp_url;
	for ( std::vector<Route>::iterator it = conf.routes.begin(); it != conf.routes.end(); ++it )
	{
		std::string test_url = finish_by_only_one( this->url, '/' );
		if ( strncmp( test_url.c_str(), it->location.c_str(), it->location.size() - 1 ) == 0 )
		{
			this->route = &(*it);
			tmp_url = this->route->root
				+ test_url.substr( test_url.find_first_of( it->location ) + it->location.size() );
			tmp_url = tmp_url.substr(0, tmp_url.size()-1);
		}
	}
	this->url = tmp_url;
	/* find route */

	this->version = splitted_str.at(2);
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
Route	*Request::get_route(void) {
	return (this->route);
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

		for ( std::vector<std::string>::iterator it = this->conf.index.begin();
			it != this->conf.index.end() ; ++it )
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