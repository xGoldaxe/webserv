#include "request.hpp"

Request::~Request(void)
{
	// if ( this->body_file )
	// 	delete this->body_file;
}

Request::Request(void)
{
	this->request_validity = false;
	this->body_file = NULL;
	this->error_status = 0;
}

/* end coplien */
std::string Request::getMethod(void) const
{
	return (method);
}
std::string Request::getBody(void)
{
	return (body);
}

std::string Request::get_legacy_url(void) const
{
	return this->legacy_url;
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

void	Request::set_status( int status_code, std::string error_message )
{
	this->error_status = status_code;
	this->error_message = error_message;
	this->request_validity = false;
}

std::pair<int, std::string>	Request::get_status(void) const
{
	return std::pair<int, std::string>( this->error_status, this->error_message );
}

Request::Request( Request const &src )
{
	*this = src;
}

Request &   Request::operator=( Request const & rhs )
{
	if (&rhs == this)
		return (*this);
	this->conf = rhs.conf;
	this->method = rhs.method;
	this->legacy_url = rhs.legacy_url;
	this->headers = rhs.headers;
	this->body = rhs.body;
	this->version = rhs.version;
	this->request_validity = rhs.request_validity;
	this->auto_index = rhs.auto_index;
	this->route = rhs.route;
	this->env = rhs.env;
	this->request_validity = rhs.request_validity;
	this->body_length = rhs.body_length;
	this->error_status = rhs.error_status;
	return (*this);
}

bool	Request::is_fulfilled() const
{
	return ( this->request_validity == true
				&& this->body.size() == this->body_length );
}

#include <fstream> 
std::ofstream	*create_unique_file( std::string path )
{
	static int i = 0;
	
	std::ofstream *File = new std::ofstream;
	File->open( std::string(path + to_string(i) + ".mem").c_str() );
	if ( File->is_open() )
		std::cout << std::string(path + to_string(i) + ".mem") << std::endl;
	else
		std::cout << "cant open" << std::endl;
	++i;
	return File;
}
/* return the amount of char added to the body */
std::size_t	Request::feed_body( std::string add_str )
{
	if ( this->body_file == NULL )
		this->body_file = create_unique_file( "memory/" );

	if ( add_str.size() == 0 )
		return 0;

	std::cout << this->body_file->is_open();

	std::size_t missing = std::min( this->body_length, add_str.size() );
	
	std::cout << "body part [" << add_str.substr( 0, missing ) << "]" << std::endl;
	*(this->body_file) << add_str.substr( 0, missing );
	this->body_length -= missing;
	return missing;
}