#include "connection.hpp"
#include "unistd.h" /** @todo */

#define ONREAD_TIMEOUT 45
#define IDLE_TIMEOUT 60
#define MAX_SIZE 5000
#define MAX_REQUESTS 5

/*************************
* @error case functions
* ***********************/
bool	Connection::is_timeout(void)
{
	time_t now = time(NULL);

	if ( this->_raw_data.size() > 0 || this->_req != NULL )
		return ( now - this->_begin_time > ONREAD_TIMEOUT );
	return ( now - this->_begin_time > IDLE_TIMEOUT );
}

/*************************
* @important functions
* ***********************/
Request	*Connection::extract_request()
{
	if ( this->_requests.empty() || this->_is_sending_data == true )
		return NULL;
	Request *res = this->_requests.front();
	this->_is_sending_data = true;
	this->_requests.pop();
	return (res);
}

/* like a get next line, but a "bit" more complex */
void	Connection::read_data()
{
	char    buff[1024];
	bzero( buff, 1024 );
	recv( this->_fd, buff, 1024 - 1, MSG_DONTWAIT );
	if ( this->_is_dead )
		return ;
	// if we start to write a new request, we reset the timer!
	if ( this->_raw_data.size() == 0 )
	{
		this->_begin_time = time(NULL);
	}
	this->_raw_data += buff;
	this->_is_new_data = true;
}

bool	Connection::check_state()
{
	int error_status = 0;
	std::string error_message;

	if ( this->is_timeout() == true )
	{
		std::cout << "trigger timeout" << std::endl; /** @todo */
		error_status = 408;
		error_message = "Request Time-out";
	}
	if ( this->_raw_data.size() > MAX_SIZE )
	{
		std::cout << "trigger max size" << std::endl; /** @todo */
		error_status = 413;
		error_message = "Request Entity Too Large";
	}
	if ( this->_requests.size() > MAX_REQUESTS )
	{
		std::cout << "trigger too much requests" << std::endl; /** @todo */
		error_status = 429;
		error_message = "Too Many Requests";
	}
	if ( error_status != 0 )
	{
		if ( this->_req != NULL )
			delete this->_req;
		this->_req = new Request();
		this->_req->set_status( error_status, error_message );
		//we delete all request and add only one
		while( this->_requests.empty() == false )
		{
			delete this->_requests.front();
			this->_requests.pop();
		}
		this->_requests.push( this->_req );
		this->soft_clear();
		this->_raw_data = "";

		this->_is_dead = true;
		return false;
	}
	return true;
}

void	Connection::queue_iteration(std::vector<Route> routes)
{
	if (this->_is_dead)
		return ;

	bool	keep_going = true;
	while ( keep_going )
	{
		keep_going = false;

		if (!this->check_state())
			break ;
		// we can create a request
		if (this->is_ready() && !this->_is_init)
		{
			this->init_request(routes);
			this->_raw_data = this->_raw_data.substr( this->_raw_data.find( "\r\n\r\n" ) + 4, this->_raw_data.size() );
		}

		// add the data to the body, only add what is required and store the remaining data
		if ( this->_is_init && this->is_invalid_req() == false )
		{
			if ( this->_req->get_state() == FEEDING )
			{
				if ( this->_data_added() )
					this->_raw_data = this->_req->feed_body( this->_raw_data ); // may invalid the request
				this->_is_new_data = false;
			}

			if ( this->_req->get_state() == PROCESSING )
				this->_req->process_file();
		}

		if ( this->_req && ( this->is_invalid_req() || this->_req->get_state() == READY ) )
		{
			this->_requests.push( this->_req );

			if (this->is_invalid_req())
			{
				this->_is_dead = true;
				this->soft_clear();
				break ;
			}

			this->soft_clear();
			keep_going = true;
		}
	}
}

void	Connection::update_timeout()
{
	this->_begin_time = std::time(0);
}

/* init with conf informations, and other usefull things for req and res */
bool	Connection::init_request(std::vector<Route> routes)
{
	Webserv_conf conf;
	this->_is_init = true;

	this->_req = new Request();
	this->_req->try_construct(this->_raw_data, routes); // set the request to valid in case of success

	return this->_req->is_request_valid();
}

/* clear only the things we dont want anymore */

void	Connection::soft_clear()
{
	this->_is_init = false;
	this->_req = NULL;
}

void	Connection::end_send()
{
	this->_is_sending_data = false;
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
	return this->_req && this->_req->is_request_valid() == false;
}

bool	Connection::_data_added()
{
	return ( this->_is_new_data == true && this->_raw_data.size() > 0 );
}
/*************************
* @coplien
* ***********************/
Connection::Connection(int fd, char *client_ip, size_t response_chunk_size) : _fd(fd), _is_init(false), _client_ip(client_ip), _response_max_size(response_chunk_size)
{
	this->_begin_time = time(NULL);
	this->_req = NULL;
	this->_is_sending_data = false;
	this->_is_dead = false;
	this->_is_new_data = true;
}

Connection::Connection(Connection const &src) : _fd(src.get_fd()),
												_req(src.get_req()),
												_raw_data(src.get_data()),
												_is_init(src._is_init),
												_begin_time(src.get_time()),
												_is_sending_data( src.get_is_sending_data() ),
												_is_dead( src.get_is_dead()),
												_client_ip(src._client_ip),
												_response_max_size(src._response_max_size),
												_is_new_data( src.get_is_new_data() )
{}

Connection &	Connection::operator=( Connection const & rhs )
{
	if (&rhs == this)
		return (*this);
	this->_fd = rhs.get_fd();
	this->_req = rhs.get_req();
	this->_raw_data = rhs.get_data();
	this->_is_init = rhs.is_init();
	this->_begin_time = rhs.get_time();
	this->_is_dead = rhs.get_is_dead();
	this->_is_new_data = rhs.get_is_new_data();
	//is_sending_data;
	return *this;
}

Connection::~Connection()
{
	if ( this->_req )
		delete this->_req;
	delete_queue( this->_requests );
}

/*************************
* @getters
* ***********************/
int	Connection::get_fd( void ) const	{

	return ( this->_fd );
}

Request		*Connection::get_req( void ) const	{

	return ( this->_req );
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

bool	Connection::get_is_dead(void) const {

	return ( this->_is_dead );
}

bool	Connection::get_is_sending_data(void) const {

	return ( this->_is_sending_data );
}

std::string Connection::get_client_ip(void) const {
	return (this->_client_ip);
}

bool	Connection::get_is_new_data(void) const {

	return ( this->_is_new_data );
}