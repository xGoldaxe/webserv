#include "request.hpp"


/** @deprecated description **/
Request::~Request(void)
{
	return ;
	if ( this->body_file != NULL )
	{
		this->body_file->close();
		this->_delete_file( this->body_file_path );
		delete this->body_file;
	}
}

Request::Request(void)
{
	this->request_validity = false;
	this->body_file = NULL;
	this->error_status = 0;
	this->fulfilled = false;
	this->body_transfer = NO_BODY;
	this->fulfilled = true;
}

/* end coplien */
std::string Request::getMethod(void) const
{
	return (method);
}

std::string Request::get_query(void) const
{
	return (this->query);
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
std::string Request::get_body_file(void) const
{
	return (this->body_file_path);
}
long long int	Request::get_body_length( void ) const
{
	return this->body_length;
}
int	Request::get_body_transfer(void) const
{
	return this->body_transfer;
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
	this->query = rhs.query;
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
	this->body_file_path = rhs.body_file_path;

	/** @attention <!-- this value is not copied --!> **/
	this->body_file = NULL;
	return (*this);
}

bool	Request::is_fulfilled() const
{
	return this->request_validity && this->fulfilled;
}

#define CGI_FILES_PATH "memory/"

#include <fstream>
//invalid if we trigger MAX_INT request in 1s, we assume its just impossible
std::ofstream	*Request::create_unique_file()
{
	static int i = 0;
	
	std::string filename = std::string( 
		+ CGI_FILES_PATH
		+ std::string( "mem." )
		+ to_string(i)
		+ "."
		+ time_to_str( time(NULL) ) );
	std::ofstream *File = new std::ofstream( filename.c_str() );

	if ( File->is_open() == false )
	{
		std::cout << "cant open " << filename << std::endl;
		delete File;
		return NULL;
	}

	std::cout << std::string( filename ) << std::endl;
	this->body_file_path = filename;
	this->_add_file( filename );
	
	++i;
	return File;
}

int	Request::write_on_file( std::string str )
{
	std::cout << "||" << str << "||" << std::endl;
	*(this->body_file) << str;
	return str.size();
}

std::string	Request::store_length( std::string add_str )
{
	long long int t = static_cast<long long int>(add_str.size());
	std::size_t missing = std::min( this->remain_body_length, t );
	
	std::string substring = add_str.substr( 0, missing );
	this->remain_body_length -= this->write_on_file( substring );

	if ( this->remain_body_length == 0 )
	{
		this->body_file->close();
		this->fulfilled = true;
	}
	return add_str.substr( missing, add_str.size() );
}

std::string	Request::store_chunk( std::string chunk_str )
{
	try
	{
		std::string res = this->chunk_buffer.feed_chunk( chunk_str );
		if ( this->chunk_buffer.is_valid() )
		{
			if ( this->chunk_buffer.is_last() )
			{
				this->fulfilled = true;
				this->body_file->close();
				std::string ins;
				ins += ulToStr( this->body_length );
				this->headers.insert( std::pair<std::string, std::string>( "Content-Length", ins ) );
			}
			else
			{
				this->body_length = this->write_on_file( this->chunk_buffer.get_body() );
				this->chunk_buffer.clean();
			}
		}
		return res;
	}
	catch(const HTTPError& e)
	{
		this->set_status( e.getCode(), e.getDescription() );
	}
	return chunk_str;
}

/* return the amount of char added to the body, may invalid the request */
std::string	Request::feed_body( std::string add_str )
{
	if ( this->body_file == NULL )
		this->body_file = this->create_unique_file();
	if ( this->body_file == NULL )
	{
		this->set_status( 500, "Internal Server Error" );
		return add_str;
	}

	if ( add_str.size() == 0 )
		return add_str;

	if ( this->body_transfer == LENGTH )
		return this->store_length( add_str );
	else if ( this->body_transfer == CHUNKED )
		return this->store_chunk( add_str );
	return add_str;
}

/* files management */
void	Request::_add_file( std::string filename )
{
	if ( filename != "" )
		Request::_created_files.push_back( filename );
}

void	Request::_delete_file( std::string filename )
{
	std::vector<std::string>::iterator	it = std::find(
		Request::_created_files.begin(),
		Request::_created_files.end(),
		filename
	);
	if ( it != Request::_created_files.end() )
	{
		Request::_created_files.erase( it );
		remove( filename.c_str() );
	}
}

void	Request::delete_all_files()
{
	for ( std::vector<std::string>::iterator it = Request::_created_files.begin();
	it != Request::_created_files.end(); ++it )
	{
		remove( it->c_str() );
		std::cout << "rm " << *it << std::endl;
	}
	Request::_created_files.clear();
}

std::vector<std::string>	Request::_created_files = std::vector<std::string>();