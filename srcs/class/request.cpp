#include "request.hpp"

Request::~Request(void)
{}

/* end coplien */
std::string Request::getMethod(void) const
{
	return (method);
}
std::string Request::getBody(void)
{
	return (body);
}
std::string Request::getUrl(void)
{
	return (url);
}
std::string Request::get_legacy_url(void)
{
	return this->legacy_url;
}
std::string Request::getRelativeUrl(void)
{
	std::cout << "out of date" << std::endl;
	return (this->conf.getServers()[0].getRoot() + url);
}
std::string Request::get_http_version(void) const
{
	return this->version;
}
Route Request::get_route(void)
{
	return (this->route);
}

bool Request::is_request_valid(void) const
{
	return request_validity;
}

bool Request::is_allowed_method(const std::string &method) const
{

	return (std::find(this->route.get_methods().begin(), this->route.get_methods().end(), method) != this->route.get_methods().end());
}

std::string	store_cat_test( bool mode, std::string value = std::string() ) {

	static std::string value_stored = std::string();

	if ( mode == true )
		value_stored = value;
	return (value_stored);
}

bool	cat_test( std::string it, std::string &res )
{
	res = store_cat_test( false ) + it;
	return ( is_file( res ) == IS_FILE_NOT_FOLDER );
}

void	Request::check_file_url(void)
{
	// this->route.auto_index = false; /** @todo NEED TO DO THIS! */
	if ( /* this->route.auto_index && */ is_file( this->url ) == IS_FILE_FOLDER )
		this->auto_index = true;
	else if ( is_file( this->url ) == IS_FILE_NOT_FOLDER )
	{
		if ( !file_readable( this->url ) )
			throw HTTPCode403();
	}
	else
	{
		store_cat_test( true, finish_by_only_one( this->url, '/' ) );
		this->url = go_through_it_until(
			this->conf.getServers()[0].getIndex(), /** @todo NEED TO DO THIS! */
			&cat_test,
			HTTPCode404()
		);
	}
}

bool	Request::is_redirection( std::string &redir_str ) {

	try
	{
		redir_str = this->route.get_redirections().at( 
			this->get_legacy_url() );
		return true;
	}
	catch(const std::exception& e)
	{
		return false;
	}
}

/* this function is use to route the url, and test many case ( redirection, cache ... )
and if no conditions are check it goes to check the file to serve and throw an error if its fail */
/* each case work as block that can be interchanged ( except the last one ) */
/* nous on a que deux cas a gerer, redirection et la fallback */

void Request::try_url( Response & res ) {

	try
	{
		std::string redir_str;
		if ( is_redirection( redir_str ) ) {

			res.set_status( 301, "Moved Permanently" );
			res.add_header( "Location", redir_str );
			return ;
		}

		// may throw errors
		this->check_file_url();
		res.set_status( 200, "OK" );
		res.load_body( *this );
		http_header_content_type( *this, res );
	} 
	catch (const HTTPError &e) {
		res.set_status( e.getCode(), e.getDescription() );
		res.error_body();
	} 
}