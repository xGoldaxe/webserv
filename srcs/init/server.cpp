#include "server.hpp"

bool	Server::close_connection( int client_socket )
{
	close( client_socket );
	std::map<int, Connection>::iterator it = this->_connections.find( client_socket );
	bool ret_val = (it != this->_connections.end());
	if ( ret_val )
	{
		this->_connections.erase( it );
		// #ifdef DEBUG
			std::cout << "Client close remote: " << client_socket << std::endl;
		// #endif
	}
	return ret_val;
}

void    Server::read_connection( int client_socket )
{
	char buffer[32];
	if ( recv(client_socket, buffer, sizeof(buffer), MSG_PEEK | MSG_DONTWAIT) == 0 ) /**  Récupérer la valeur de retour et couper la connexion si -1, je pense pas que cq soit necessqire ici */
	{
		this->close_connection( client_socket );
		return ;
	}

	std::map<int, Connection>::iterator client_socket_it = this->_connections.find(client_socket);
	if (client_socket_it != this->_connections.end())
		client_socket_it->second.read_data();
	else
		close(client_socket);
}


void    Server::add_response( Request * req, int fd )
{
	std::string hostname = req->get_header_value("Host");
	Server_conf serv = this->_virtual_servers.get_server_from_server_name(hostname);

	Response *res = new Response( fd, serv.getIndex(), req, this->_socket_addr_eq[fd].c_str(), serv.getBodyMaxSize());

	if ( req->is_request_valid() )
	{
		res->try_url(this->_connections.at(fd).get_client_ip());
		/* generic headers */
		res->add_header( "Connection", "keep-alive" );
		res->add_header("Keep-Alive", "timeout=5, max=10000");
	}
	else
	{
		res->add_header( "Connection", "close" );
		res->error_body();
	}

	http_header_date( *req, *res );
	http_header_server( *req, *res );

	int shouldQueue = res->send();
	if (shouldQueue >= 0) {
		this->_queue.push(res);
	} 
	else {
		std::map<int, Connection>::iterator it_connection = this->_connections.find( res->client_socket );
		if (it_connection != this->_connections.end()) {
			it_connection->second.end_send();
		}
		this->_close_queue.push( it_connection->first );
		res->output(this->_request_handled++);
		delete res; // will also delete req
		res = NULL;
		req = NULL;
	}
}

void  Server::trigger_queue( void )
{
	for ( std::map<int, Connection>::iterator it = this->_connections.begin(); it != this->_connections.end(); ++it )
	{
		it->second.queue_iteration(this->_virtual_servers);
		Request *req = it->second.extract_request();
		if ( req != NULL )
			this->add_response( req, it->first );
	}
	//delete some connections
	while ( this->_close_queue.empty() != true )
	{
		this->close_connection( this->_close_queue.front() );
		this->_close_queue.pop();
	}
}

/* wont wait for connection anymore, instead we will alternate from Connection_queue and epoll */
void    Server::wait_for_connections( void )
{
	struct epoll_event evlist[ this->BACKLOG ];
	int nbr_req = epoll_wait(this->_poll_fd, evlist, this->BACKLOG, 0);
	for (int i = 0; i < nbr_req; ++i) { 
		this->read_connection(evlist[i].data.fd);
	}
}

#define RUN_FOLDER "./memory/"
Server::Server(char **env, Bundle_server bundle) : _request_handled(0),
													_virtual_servers(bundle),
													_env(env),
													// _server_name(serv_conf.getName()),
													_host(bundle.getHost())
													// _index(serv_conf.getIndex()),
													// _body_max_size(serv_conf.getBodyMaxSize()),
													// _root(serv_conf.getRoot()),
													// _routes(serv_conf.getRoutes()),
													// _error_pages(serv_conf.error),
													// _onread_Timeout(serv_conf.getReadTimeOut()),
													// _server_body_size(serv_conf.getServerBodySize()),
													// _client_header_size(serv_conf.getClientHeaderSize()),
{
	s_server_addr_in addr;
	std::memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(bundle.getPort());
	
	if (inet_aton(this->_host.c_str(), &addr.sin_addr) == 0) {
		throw std::invalid_argument("Unexisting host");
	}
	this->_addr = addr;

	this->_is_init = false;
	this->_queue = std::queue<Response *>();

	this->BACKLOG = 1024; /** @todo bundle.getMaxWorkers() **/
	this->_run_folder = RUN_FOLDER; /** @todo bundle.runFolder() **/
	this->_server_conf = bundle.getMainServer();

	this->_create_run_folder();
}

Server::Server(const Server &rhs) : _addr(rhs._addr),
									_socket_fd(rhs._socket_fd),
									_poll_fd(rhs._poll_fd),
									_connections(rhs._connections),
									_c_queue(rhs._c_queue),
									_queue(rhs._queue),
									_request_handled(rhs._request_handled),
									_env(rhs._env),
									_is_init(rhs._is_init),
									_server_name(rhs._server_name),
									_host(rhs._host),
									_index(rhs._index),
									_body_max_size(rhs._body_max_size),
									_root(rhs._root),
									_routes(rhs._routes),
									_onread_Timeout(rhs._onread_Timeout),
									_server_body_size(rhs._server_body_size),
									_client_header_size(rhs._client_header_size)
{
	this->_is_init = false;
}

Server::~Server()
{
	if (this->_is_init) {
		close(this->_poll_fd);
		close(this->_socket_fd);

		delete_queue( this->_queue );
		delete_queue( this->_c_queue );

		std::cout << "Server closed." << std::endl;
	}
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
	this->_is_init = true;

	int sock = socket(AF_INET, SOCK_STREAM, 0);
	this->_socket_fd = sock;
	int set_opt = 1;
	setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &set_opt, sizeof(int));
	setsockopt(sock, SOL_SOCKET, SO_REUSEPORT, &set_opt, sizeof(int));

	this->_bind_port(sock, this->_addr);

	if (listen(sock, this->BACKLOG) < 0)
		throw ServerNotListeningException();

	this->_report(sock, this->_addr);
	fcntl(sock, F_SETFL, O_NONBLOCK);

	this->_poll_fd = epoll_create1(O_CLOEXEC);
	this->_poll_socket_eq.insert(std::pair<int, int>(this->_poll_fd, sock));
}

void    Server::handle_responses()
{
	std::queue<Response *> new_queue;

	for (int runner_i = 0; !this->_queue.empty() && runner_i < MAX_RUNNERS && !shouldQuit(); runner_i++)
	{
		Response *res = this->_queue.front();

		if (this->_connections.find(res->client_socket) != this->_connections.end())
		{
			std::map<int, Connection>::iterator it = this->_connections.find(res->client_socket);

			int status_res = 0;
			it->second.update_timeout();
			if ((status_res = res->send_chunk()) > 0)
			{
				new_queue.push(res);
			}
			else if (status_res <= 0)
			{
				if (status_res == 0) {
					std::string response_content = "0\r\n\r\n";
					int status = ::send(res->client_socket, response_content.c_str(), response_content.length(), 0);
					if (status < 0)
						this->close_connection(res->client_socket);
				}

				res->output(this->_request_handled++);

				it->second.end_send();
				if (it->second.get_is_dead() || status_res < 0)
					this->close_connection(res->client_socket);
				delete res;
			}
		}
		else
			delete res;

		this->_queue.pop();
	}

	while (!this->_queue.empty() && !shouldQuit()) {
		new_queue.push(this->_queue.front());
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
		int client_socket = accept(this->_socket_fd, (struct sockaddr *)&cli_addr, &clilen);
		if (client_socket == -1)
			break;

		if ( this->_connections.size() > static_cast<std::size_t>(this->BACKLOG) )
		{
			close(client_socket);
			continue;
		}

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
		ev.events = EPOLLIN | EPOLLOUT;
		ev.data.fd = client_socket;
		epoll_ctl(this->_poll_fd, EPOLL_CTL_ADD, client_socket, &ev);

		// #ifdef DEBUG
			std::cout << "new connection: " << client_socket << std::endl;
		// #endif
		this->_connections.insert(
			std::pair<int, Connection>(client_socket,
				Connection(client_socket, inet_ntoa(cli_addr.sin_addr), this->_server_conf )));
		this->_socket_addr_eq[client_socket] = inet_ntoa(cli_addr.sin_addr);
	}
}

void Server::_report(int sock, s_server_addr_in server_addr)
{
	char host_buffer[INET6_ADDRSTRLEN];
	char service_buffer[NI_MAXSERV];
	socklen_t addr_len = sizeof(server_addr);
	int err = getnameinfo(
		(s_server_addr)&server_addr,
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
	this->_socket_addr_eq.insert(std::pair<int, std::string>(sock, std::string(host_buffer) + ":" + std::string(service_buffer)));
	std::cout << "\tServer listening on http://" << host_buffer << ":" << service_buffer << std::endl;
}

void Server::_bind_port(int sock, s_server_addr_in server_addr)
{
	if (bind(sock, (s_server_addr)&server_addr, sizeof(server_addr)) == -1)
	{
		char in_addr[INET_ADDRSTRLEN];
		inet_ntop(AF_INET, &(server_addr.sin_addr), in_addr, INET_ADDRSTRLEN);

		std::cerr << "Address:Port are already used (or missing permissions) " << in_addr << ":" << ntohs(server_addr.sin_port) << std::endl;
		
		throw ServerNotListeningException();
	}
}

size_t Server::countHandledRequest()
{
	return this->_request_handled;
}

char asciitolower(char in) {
	if (in <= 'Z' && in >= 'A')
		return in - ('Z' - 'z');
	return in;
}

bool Server::is_a_server_name(std::string server_name)
{
	std::vector<std::string>::iterator it;

	for (it = this->_server_name.begin(); it != this->_server_name.end(); it++)
	{
		if (server_name.compare(*it) == 0)
			return true;
	}
	return false;
}

bool Server::is_server_name(std::string hostname)
{
	std::transform(hostname.begin(), hostname.end(), hostname.begin(), asciitolower);
	if (this->is_a_server_name(hostname)) {
		return true;
	}
	return false;
}

void    Server::_create_run_folder()
{
	struct stat st;

	if ( stat( this->_run_folder.data(), &st) == -1 )
	{
		mkdir( this->_run_folder.data(), 0700);
	}
	if ( stat( this->_run_folder.data(), &st) == -1 )
	{
		std::cerr << "Can't create memory folder." << std::endl;
	}
}