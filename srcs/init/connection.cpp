#include "connection.hpp"
#include "unistd.h"

#define ONREAD_TIMEOUT 30
#define IDLE_TIMEOUT 60
/*************************
* @erroer case functions
* ***********************/
bool	Connection::is_timeout(void)
{
	time_t now = time(NULL);
	if ( this->_raw_data.size() > 0 )
	{
		if ( now - this->_begin_time > ONREAD_TIMEOUT )
		{
			std::cout << "on_read" << std::endl;
			return true;
		}
	}
	if ( now - this->_begin_time > IDLE_TIMEOUT )
	{
		std::cout << "on_idle" << std::endl;
		return true;
	}
	return false;
}

/*************************
* @important functions
* ***********************/
/* like a get next line, but a "bit" more complex */
void	Connection::add_data( char * buffer )
{
	// if we start to write a new request, we reset the timer!
	if ( this->_raw_data.size() == 0 )
		this->_begin_time = time(NULL);
	this->_raw_data += buffer;
}

Response	*Connection::queue_iteration()
{
	// error case
	/*
	if size of raw_data > MAX_HEADERS_SIZE
		throw HTTP ERROR
	if request isnt ready AND start request longere than TIMEOUT_MAX
		throw HTTP ERROR 408 and close connection from socket ( at catch level )
	*/

	// we can create a request
	if ( this->is_ready() )
	{
        this->init_request();
		this->_raw_data = this->_raw_data.substr( this->_raw_data.find( "\r\n\r\n" ) + 4, this->_raw_data.size() );
		std::cout << "remain {" << this->_raw_data << "}" << std::endl;
	}

	// add the data to the body, only add what is required and store the remaining data
	if ( this->_is_init && this->is_invalid_req() == false )
	{
		std::size_t read_until = this->_req.feed_body( this->_raw_data );
		this->_raw_data = this->_raw_data.substr( read_until, this->_raw_data.size() );
	}

    if ( this->is_invalid_req() || this->is_fulfilled() )
	{
        return this->process();
		this->soft_clear();
	}

	/* maybe we have enough data to run a second consecutive request, perhaps this is
	extremely dangerous, a user could chain many request and take all the trafic for a while.
	Since the minimal request is "GET / HTTP/1.1\r\n\r\n"(19 bytes), and each read can be over 1000 bytes.
	we have to process all data already reader before accept connections again. So lets create a queue with
	all Connections with "remaining data"*/
	return NULL;
}

/* init with conf informations, and other usefull things for req and res */
bool	Connection::init_request()
{
	Webserv_conf conf;
	std::cout << this->_raw_data;
	this->_is_init = true;
	try
	{
		this->_req.try_construct( this->_raw_data, conf );
		this->_res = new Response( this->_fd, conf, &this->_req );
		std::cout << "valid request" << std::endl;
		return true;
	}
	catch(const std::exception& e)
	{
		std::cout << "not valid request" << std::endl;
		return false;
	}
}

/* all big work happen here */

Response	*Connection::process()
{
	Webserv_conf conf;
	
	http_header_date( this->_req, *this->_res );
	http_header_server( this->_req, *this->_res );
	if ( this->_req.is_request_valid() )
	{
		this->_req.try_url( *this->_res );
		/* generic headers */
		this->_res->add_header( "Connection", "keep-alive" );
		this->_res->add_header("Keep-Alive", "timeout=5, max=10000");
		this->_res->send();
	}
	else
	{
		this->_res->set_status( 400, "Bad Request" );
		this->_res->send();
		close( this->_res->client_socket );
	}
	this->_begin_time = time(NULL);
	return this->_res;
}

/* clear only the things we dont want anymore */

void	Connection::soft_clear()
{
	this->_req = Request();
	this->_is_init = false;
}

/*************************
* @checkers
* ***********************/
bool	Connection::is_ready( void )
{
    std::size_t EOF_index = this->_raw_data.find("\r\n\r\n");
	return ( EOF_index != std::string::npos );
}

bool	Connection::is_invalid_req()
{
	return this->_is_init && this->_req.is_request_valid() == false;
}

bool	Connection::is_fulfilled()
{
	return this->_req.is_request_valid() && this->_req.is_fulfilled();
}


/*************************
* @coplien
* ***********************/
Connection::Connection( int fd ) : _fd( fd ), _is_init( false )
{
	this->_begin_time = time(NULL);
}

Connection::Connection(Connection const &src) : _fd(src.get_fd()),
												_req(src.get_req()),
												_res(src.get_res()),
												_raw_data(src.get_data()),
												_is_init(src._is_init),
												_begin_time(src.get_time())
{}

Connection &	Connection::operator=( Connection const & rhs )
{
	this->_fd = rhs.get_fd();
	this->_req = rhs.get_req();
	this->_res = rhs.get_res();
	this->_raw_data = rhs.get_data();
	this->_is_init = rhs.is_init();
	this->_begin_time = rhs.get_time();
	return *this;
}

Connection::~Connection()
{}

/*************************
* @getters
* ***********************/
int	Connection::get_fd( void ) const	{

	return ( this->_fd );
}

Request		Connection::get_req( void ) const	{

	return ( this->_req );
}

Response	*Connection::get_res( void ) const	{

	return ( this->_res );
}

std::string	Connection::get_data( void ) const	{

	return ( this->_raw_data );
}

bool	Connection::is_init( void ) const	{

	return ( this->_is_init );
}

time_t	Connection::get_time( void ) const	{
	
	return ( this->_begin_time );
}