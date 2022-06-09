#include "request.hpp"

Request::~Request(void)
{
	if ( this->body_file != NULL )
		delete this->body_file;
}

Request::Request(void)
{
	this->request_validity = false;
	this->body_file = NULL;
	this->error_status = 0;
	this->fulfilled = false;
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
std::string Request::get_header_value(std::string name) const
{
	std::map<std::string, std::string>::const_iterator it = this->headers.find(name);
	if (it == this->headers.end())
		return "";
	return this->headers.at(name);
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
	this->auto_index = rhs.auto_index;
	this->route = rhs.route;
	this->env = rhs.env;
	this->request_validity = rhs.request_validity;
	this->body_length = rhs.body_length;
	this->error_status = rhs.error_status;
	this->body_transfer = rhs.body_transfer;
	this->fulfilled = rhs.fulfilled;
	this->error_message = rhs.error_message;

	/** @attention <!-- this value is not copied --!> **/
	this->body_file = NULL;
	return (*this);
}

bool	Request::is_fulfilled() const
{
	return this->request_validity && this->fulfilled;
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

#define MAX_CHUNK_SIZE 200
/*read chunk*/
/*
WE NEED MAX_CHUNK_SIZE
HEXA TO INT since /r/n ( if other char than hexa throw 400 )
0123456789ABCDEF[\r\n]
x*[char][\r\n]

//didnt throw anything, just set himself to error
*/
std::size_t	Request::store_chunk( std::string chunk_str )
{
	// throw 400 in case of invalid chunk, true for full chunk and false for partial chunk
	// int size;
	// int	whole_chunk_size;
	// std::string content;
	(void) chunk_str;
	this->set_status( 413, "Payload Too Large" );
	this->fulfilled = false;
	//if 0 size chunk
		//this->fulfilled = true;
	// bool is_full = try_chunk( &size, &content, &whole_chuck_size );
	// if ( is_full )
	// {
	// 	if ( is_full && size > MAX_CHUNK_SIZE ) too big // not enoguh verification
	// 		throw 413
	// 	return whole_chunk_size;
	// }
	// 	==> check_size
	// 		throw 413
	// 	...
	return 0;
}

std::size_t	Request::store_length( std::string add_str )
{
	std::size_t missing = std::min( this->body_length, add_str.size() );
	
	std::cout << "body part [" << add_str.substr( 0, missing ) << "]" << std::endl;
	*(this->body_file) << add_str.substr( 0, missing );
	this->body_length -= missing;

	if ( this->body_length )
		this->fulfilled = true;

	return missing;
}

/* return the amount of char added to the body, may invalid the request */
std::size_t	Request::feed_body( std::string add_str )
{
	if ( this->body_file == NULL )
		this->body_file = create_unique_file( "memory/" );
	if ( this->body_file->is_open() == false )
	{
		this->set_status( 500, "Internal Server Error" );
		return 0;
	}

	if ( add_str.size() == 0 )
		return 0;

	if ( this->body_transfer == LENGTH )
		return this->store_length( add_str );
	else if ( this->body_transfer == CHUNKED )
		return this->store_chunk( add_str );
	return 0;
}