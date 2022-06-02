#include "../webserv.hpp"
#include <algorithm>
#include <string.h>

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
Route	Request::get_route(void) {
	return (this->route);
}

bool	Request::is_request_valid(void) const {
	return request_validity;
}

bool	Request::is_allowed_method( const std::string &method ) const {

	return ( std::find( this->route.methods.begin(), this->route.methods.end(), method) != this->route.methods.end() );
}

std::string Request::try_url( Response & res ) {

	res.set_status( 404, "Not Found" ); // fallback if 0 condition has been checked

	try
	{
		std::string	redir = this->route.redirections.at( 
			this->get_legacy_url() );

		res.set_status( 301, "Moved Permanently" );
		res.add_header( "Location", redir );
		return ( this->url );
	}
	catch( const std::exception& e ) {}

	if ( this->route.auto_index && is_file( this->url.c_str() ) == 0 ) // this is a folder
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