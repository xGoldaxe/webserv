#pragma once

#include <cstring>
#include <unistd.h>

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

#include <queue>

#include "exception_server_not_listening.hpp"
#include "configuration/webserv.hpp"

class Connection;
#include "connection.hpp"

#define BACKLOG 20
#define MAX_RUNNERS 20

#include "class/response.hpp"

typedef struct sockaddr_in s_server_addr_in;
typedef const struct sockaddr* s_server_addr;

class Response;

class Server {
public:
	Server(char **env, Server_conf serv_conf);
	Server(const Server &rhs);
	~Server();
	void init_connection();
	void handle_client();
	void handle_responses();
	void wait_for_connections();
	void trigger_queue();
	bool queue_response(Response *res);
	size_t countHandledRequest();

	// Getters
	std::vector<int> get_socket() const;
	std::vector<int> get_poll_fd() const;

private:
	// Internal variables
	std::vector<s_server_addr_in>	_addrs;
	std::vector<int> 				_socket_fds;
	std::vector<int>				_poll_fds;
	std::map<int, Connection>		_connections;
	std::map<int, std::string>		_socket_addr_eq;
	std::map<int, int>				_poll_socket_eq;
	std::queue<Connection*>			_c_queue;
	std::queue<Response *>   		_queue;
	size_t                   		_request_handled;
	char							**_env;
	bool							_is_init;

	// Configuration
	std::string						_server_name;
	std::string						_host;
	std::vector<std::string>		_index;
	int								_body_max_size;
	std::string 					_root;
	std::vector<Route>				_routes;
	std::map<int, std::string>		_error_pages;
	int								_read_timeout;
	int								_server_body_size;
	int								_client_header_size;

	// Methods
	Server();

	void _report(int sock, s_server_addr_in server_addr);
	void _bind_port(int sock, s_server_addr_in server_addr);

	void read_connection(int client_socket);
	bool close_connection(int client_socket);

	void add_response(Request *req, int fd);
};
