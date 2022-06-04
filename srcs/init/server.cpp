#include "server.hpp"

#include <strings.h>
// #include "../class/webserv_conf.hpp"
// #include "../class/request.hpp"
// #include "../class/response.hpp"
#include "../webserv.hpp"

void    Server::read_connection( int client_socket )
{
    char buffer[32];
    if ( recv(client_socket, buffer, sizeof(buffer), MSG_PEEK | MSG_DONTWAIT) == 0 )
	{
		std::cout << "Client close remote: " << client_socket << std::endl;
		close( client_socket );
        if ( this->_raw_request_map.find( client_socket ) != this->_raw_request_map.end() )
            this->_raw_request_map.erase( this->_raw_request_map.find( client_socket ) );
        if ( this->_requests.find( client_socket ) != this->_requests.end() )
            this->_requests.erase( this->_requests.find( client_socket ) );
		return ;
	}

	char    buff[1024];
	bzero( buff, 1024 );
	recv( client_socket, buff, 1024 - 1, 0 );
	this->_raw_request_map[client_socket] += buff;


    std::size_t EOF_index = this->_raw_request_map[client_socket].find("GET");
    std::cout << EOF_index << std::endl;
    if ( EOF_index != std::string::npos )
    {
        Webserv_conf	conf; 

        std::cout << "{" << std::endl << this->_raw_request_map[client_socket] << "}" << std::endl;
        Request req( this->_raw_request_map[client_socket], conf );
        this->_requests[client_socket] = req;

        if ( this->_requests[client_socket].add_body( 
            this->_raw_request_map[client_socket].substr( 
                EOF_index, 
                this->_raw_request_map[client_socket].size() 
        ) ) )
        {
            Response res( client_socket, conf, this->_requests[client_socket] );
            http_get_response( this->_requests[client_socket], res);
            this->_raw_request_map[client_socket] = "";
        }
    }
	// // req.env = env;
}

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

Server::Server()
{
    this->_socket_fd = socket(AF_INET, SOCK_STREAM, 0);

    this->_port = 3000;

     this->_addr.sin_family = AF_INET;
    this->_addr.sin_port = htons(3000);
    this->_addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

    this->_select_port();

    if (listen(this->_socket_fd, BACKLOG) < 0)
        throw new ServerNotListeningException;

    this->_report(&this->_addr);
}

Server::~Server()
{
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
    this->_poll_fd = epoll_create1(O_CLOEXEC);
    fcntl(this->get_socket(), F_SETFL, O_NONBLOCK);
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
        std::cout << "========>new registered connection!<========" << std::endl;
    }
}

short Server::_select_port()
{
    while (bind(this->_socket_fd, (s_server_addr)&this->_addr, sizeof(this->_addr)) == -1)
    {
        this->_addr.sin_port = htons(++this->_port);
        #ifdef DEBUG
            std::cout << this->_port << std::endl;
        #endif
    }

    return this->_port;
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
    std::cout << "\n\n\tServer listening on http://" << host_buffer << ":" << service_buffer << std::endl;
}
