#pragma once
#include "../webserv.hpp"
#include "connection.hpp"

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <netdb.h> // for getnameinfo()
#include <fcntl.h>

// Usual socket headers
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include <netinet/tcp.h> /* TCP_KEEPIDLE, etc. */
#include <arpa/inet.h>

#include <iostream>
#include <map>
#include <queue>
#include <fstream>
#include <string>
#include <sys/epoll.h>
#include <sys/stat.h>

#include "exception_server_not_listening.hpp"

#define BACKLOG 10

typedef struct sockaddr_in s_server_addr_in;
typedef const struct sockaddr* s_server_addr;

class Server {
	public:
		Server();
		~Server();
		void    init_connection();
		void    handle_client();
		void    wait_for_connections();
		void	trigger_queue(void);

		// Getters
		int     get_socket() const;
		int     get_poll_fd() const;

	private:
		short						_port;
		s_server_addr_in			_addr;
		int							_socket_fd;
		int							_poll_fd;
		std::map<int, Connection>	_connections;
		std::queue<Connection*>		_c_queue;

		short    _select_port();
		void     _report( s_server_addr_in *server_addr );

		void    read_connection( int client_socket );
		bool	close_connection( int client_socket );
};
