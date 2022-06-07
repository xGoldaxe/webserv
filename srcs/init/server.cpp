#include "server.hpp"

#include <cstring>
#include "../webserv.hpp"

bool	Server::close_connection( int client_socket )
{
    #ifdef DEBUG
	    std::cout << "Client close remote: " << client_socket << std::endl;
    #endif

	close( client_socket );
	std::map<int, Connection>::iterator it = this->_connections.find( client_socket );
	bool ret_val = (it != this->_connections.end());
	if ( ret_val )
		this->_connections.erase( it );
	return ret_val;
}

void    Server::read_connection( int client_socket )
{
	char buffer[32];
	if ( recv(client_socket, buffer, sizeof(buffer), MSG_PEEK | MSG_DONTWAIT) == 0 )
	{
		this->close_connection( client_socket );
		return ;
	}

	char    buff[1024];
	bzero( buff, 1024 );
	recv( client_socket, buff, 1024 - 1, 0 );

	this->_connections.at(client_socket).add_data( buff );
}


/** @todo fix req leaks */
void    Server::add_response( Request * req, Response * res )
{
    Webserv_conf conf;
	
	http_header_date( *req, *res );
	http_header_server( *req, *res );

	if ( req->is_request_valid() )
	{
		req->try_url(res);
		/* generic headers */
		res->add_header( "Connection", "keep-alive" );
		res->add_header("Keep-Alive", "timeout=5, max=10000");
		// res->send();
	}
	else
	{
		res->set_status( 400, "Bad Request" );
		// res->send();
		// close( res->client_socket );
	}
    this->queue_response( res );
}

void  Server::trigger_queue( void )
{
	std::queue<Connection> to_close;

	for ( std::map<int, Connection>::iterator it = this->_connections.begin(); it != this->_connections.end(); ++it )
	{
        // will had to response_q by itslef
        if (it->second.queue_iteration()) {
            this->add_response(it->second.get_req(), it->second.get_res());
            it->second.soft_clear();
        }
        // can't timeout anymore if something is sent
		if (it->second.is_timeout())
        {
            std::cout << "disconected from timeout" << std::endl;
		 	to_close.push(it->second);
        }
    }
	while (!to_close.empty())
	{
        /** @todo we dont want to close the connection, send a 408 instead, it will close the connection after the message has been sent **/
		this->close_connection( to_close.front().get_fd() );
		to_close.pop();
	}
}

/* wont wait for connection anymore, instead we will alternate from Connection_queue and epoll */
void    Server::wait_for_connections( void )
{
	struct epoll_event evlist[1024];
	int nbr_req = epoll_wait( this->get_poll_fd(), evlist, 1024, 0 );
	for (int i = 0; i < nbr_req; ++i)
    {
        std::cout << "read from, fd: " << evlist[i].data.fd << std::endl;
		this->read_connection( evlist[i].data.fd );
    }
}

Server::Server() : _socket_fd(0), _poll_fd(0), _request_handled(0)
{
    this->_port = 3000;

    this->_queue = std::queue<Response *>();

    this->_addr.sin_family = AF_INET;
    this->_addr.sin_port = htons(this->_port);
    this->_addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
}

Server::~Server()
{
    if (this->_poll_fd > 0)
        close(this->_poll_fd);

    if (this->_socket_fd > 0)
        close(this->_socket_fd);

    while (!this->_queue.empty()) {
        delete this->_queue.front();
        this->_queue.pop();
    }

    std::cout << "Server closed." << std::endl;
}

int Server::get_socket() const
{
	return this->_socket_fd;
}

int Server::get_poll_fd() const
{
	return this->_poll_fd;
}

void Server::init_connection()
{
    this->_socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    bool set_opt = 1;
    setsockopt(this->_socket_fd, SOL_SOCKET, SO_REUSEADDR, &set_opt, sizeof(int));

    this->_bind_port();

    if (listen(this->_socket_fd, BACKLOG) < 0)
        throw ServerNotListeningException();

    this->_report(&this->_addr);

    this->_poll_fd = epoll_create1(O_CLOEXEC);
    fcntl(this->get_socket(), F_SETFL, O_NONBLOCK);
}

bool    Server::queue_response(Response *res)
{
    this->_request_handled++;
    if (res->get_size_next_chunk() > 0) {
        this->_queue.push(res);
    } else {
        delete res;
    }
    return true;
}

/** @todo stop looking for deconnection, instead look if connection is still alive in _connections **/
/** @todo trigger an event when everything is sent ( used mainly to close connection when its required (400, 408) ) **/
/** @todo while its sending, it must also reset the timeout of the connection **/
void    Server::handle_responses()
{
    std::queue<Response *> new_queue;

    int runner_i = 0;

    while (!this->_queue.empty() && runner_i < MAX_RUNNERS && exit_code == 0)
    {
        Response *res = this->_queue.front();
        
        /* with MSG_PEEK, no data will be ride of the socket */
        char buffer[256];
        if (recv(res->client_socket, buffer, sizeof(buffer), MSG_PEEK | MSG_DONTWAIT) == 0) {
            delete this->_queue.front();
        } else {
            size_t exchange = res->send_chunk();
            if (exchange > 0) {
                new_queue.push(res);
            } else {
                std::string response_content = "0\r\n\r\n";
                ::send(res->client_socket, response_content.c_str(), response_content.size(), 0);

                delete this->_queue.front();
            }
        }

        this->_queue.pop();

        runner_i++;
    }

    while (!this->_queue.empty() && exit_code == 0) {
        Response *res = this->_queue.front();
        new_queue.push(res);
        this->_queue.pop();
    }

    this->_queue = new_queue;
}

void Server::handle_client()
{
	struct sockaddr_in cli_addr;

	// we accept some new request
	while (true)
	{
		socklen_t clilen = sizeof(cli_addr);
		int client_socket = accept(this->get_socket(), (struct sockaddr *)&cli_addr, &clilen);
		if (client_socket == -1)
			break;

		int swtch = 1;     /* 1=KeepAlive On, 0=KeepAlive Off. */
		int idle = 7200;   /* Number of idle seconds before sending a KeepAlive probe. */
		int interval = 75; /* How often in seconds to resend an unacked KeepAlive probe. */
		int count = 9;     /* How many times to resend a KA probe if previous probe was unacked. */

		/* Switch KeepAlive on or off for this side of the socket. */
		setsockopt(client_socket, SOL_SOCKET, SO_KEEPALIVE, &swtch, sizeof(swtch));

		if (swtch)
		{
			/* Set the number of seconds the connection must be idle before sending a KA probe. */
			setsockopt(client_socket, IPPROTO_TCP, TCP_KEEPIDLE, &idle, sizeof(idle));

			/* Set how often in seconds to resend an unacked KA probe. */
			setsockopt(client_socket, IPPROTO_TCP, TCP_KEEPINTVL, &interval, sizeof(interval));

			/* Set how many times to resend a KA probe if previous probe was unacked. */
			setsockopt(client_socket, IPPROTO_TCP, TCP_KEEPCNT, &count, sizeof(count));
		}

		struct epoll_event ev;
		bzero(&ev, sizeof(ev));
		// ev.events = EPOLLET | EPOLLIN;
		ev.events = EPOLLIN;
		ev.data.fd = client_socket;
		epoll_ctl(this->_poll_fd, EPOLL_CTL_ADD, client_socket, &ev);

		this->_connections.insert( 
			std::pair<int, Connection>(client_socket, Connection( client_socket ) )
		);
        std::cout << "========>new registered connection(" << client_socket <<")!<========" << std::endl;
	}
}

void Server::_report(s_server_addr_in *server_addr)
{
    char host_buffer[INET6_ADDRSTRLEN];
    char service_buffer[NI_MAXSERV];
    socklen_t addr_len = sizeof(*server_addr);
    int err = getnameinfo(
        (s_server_addr)server_addr,
        addr_len,
        host_buffer,
        sizeof(host_buffer),
        service_buffer,
        sizeof(host_buffer),
        NI_NUMERICHOST);
    if (err != 0)
    {
        std::cout << "It's not working!" << std::endl;
    }
    std::cout << "\n\tServer listening on http://" << host_buffer << ":" << service_buffer << std::endl;
}

void Server::_bind_port()
{
    int i = 0;
    while (bind(this->_socket_fd, (s_server_addr) & this->_addr, sizeof(this->_addr)) == -1 && i < 100)
    {
        if (i % 10 == 0)
        {
            std::cerr << "Can't bind port " << this->_port << ". Retrying in 10sec. (Try " << (i / 10) << "/10)" << std::endl;
        }
        sleep(1);
        i++;
    }
}

size_t Server::countHandledRequest()
{
    return this->_request_handled;
}
