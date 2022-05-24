#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <netdb.h> // for getnameinfo()
#include <fcntl.h>

// Usual socket headers
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include <netinet/tcp.h>   /* TCP_KEEPIDLE, etc. */
#include <arpa/inet.h>

#include <iostream>
#include <fstream>
#include <string>

#include <sys/epoll.h>

#include "webserv.hpp"

#define SIZE 1024
#define BACKLOG 10 //passed to listen

void report(struct sockaddr_in *server_addr)
{
	char host_buffer[INET6_ADDRSTRLEN];
	char service_buffer[NI_MAXSERV];
	socklen_t addr_len = sizeof(*server_addr);
	int err = getnameinfo(
        (struct sockaddr *) server_addr,
        addr_len,
        host_buffer,
        sizeof(host_buffer),
        service_buffer,
        sizeof(host_buffer),
        NI_NUMERICHOST
    );
	if (err != 0) {
		std::cout << "It's not working!" << std::endl;
	}
	std::cout << "\n\n\tServer listening on http://" << host_buffer << ":" << service_buffer << std::endl;
}

void process_request(int client_socket, char **env)
{
	std::string req_raw_data;
	char buffer[256];
	bzero(buffer,256);
	int n = 255;
	while ( n > 0 )
	{
		bzero(buffer,256);
		n = read(client_socket, buffer, 255);
		buffer[n] = '\0';
		req_raw_data += buffer;
		if ( req_raw_data.size() < 2 ||
				( req_raw_data.at( req_raw_data.size() - 2 ) == '\r' &&
					req_raw_data.at( req_raw_data.size() - 1 ) == '\n' ) 
			)
			break ;
	}

	// std::cout << "<-----------{top}----------->" << std::endl;
	// std::cout << req_raw_data << std::endl;
	// std::cout << "<-----------{bottom}----------->" << std::endl;
	/* we will need further verification */
	if (req_raw_data.size() == 0)
		return ;

	webserv_conf	conf; 
	
	Request req( req_raw_data, conf );
	req.env = env;
	Response res( client_socket, conf, req );

	std::cout << "request url: " << req.getUrl() << std::endl;
	http_get_response(req, res);
}

int main(int argc, char **argv, char **env)
{
	(void) argc;
	(void) argv;
	(void) env;

	int server_socket = socket(
		AF_INET,
		SOCK_STREAM,
		0
	);

	short port = 3000;

	struct sockaddr_in	server_addr, cli_addr;
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(port);
	server_addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

	while ( bind(
		server_socket,
		(struct sockaddr *) &server_addr,
		sizeof(server_addr)
	) == -1 )
	{
		++port;
		server_addr.sin_port = htons(port);
		std::cout << port << std::endl;
	}

	int listening = listen(server_socket, BACKLOG);
	if (listening < 0){
		printf("Error: The server is not listening.\n");
		return 1;
	}
	report(&server_addr);
	int client_socket;

	///////////////////////////////
	int epfd = epoll_create1(O_CLOEXEC);
	fcntl(server_socket, F_SETFL, O_NONBLOCK);
	while(1) {
		// we accept some new request
		while (1)
		{
			socklen_t clilen = sizeof(cli_addr);
			client_socket = accept( server_socket, (struct sockaddr *) &cli_addr, &clilen );
			if (client_socket == -1)
				break;

			int swtch = 1;	/* 1=KeepAlive On, 0=KeepAlive Off. */
			int idle = 1;	/* Number of idle seconds before sending a KeepAlive probe. */
			int interval= 1;	/* How often in seconds to resend an unacked KeepAlive probe. */
			int count = 1;	/* How many times to resend a KA probe if previous probe was unacked. */

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
			bzero( &ev, sizeof(ev) );
			ev.events = EPOLLET | EPOLLIN;
			ev.data.fd = client_socket;
			epoll_ctl(epfd, EPOLL_CTL_ADD, client_socket, &ev);
			std::cout << "========>new registered connection!<========" << std::endl;
		}

		struct epoll_event evlist[1024];
		int nbr_req = epoll_wait(epfd, evlist, 1024, 0);
		for (int i = 0; i < nbr_req; ++i)
		{
			std::cout << "read from, fd: " << evlist[i].data.fd << std::endl;
			process_request( evlist[i].data.fd, env );
			// std::cout << "now closing connection, fd: " << evlist[i].data.fd << std::endl;
			// close( evlist[i].data.fd );
		}
	}
	/* connections must have a lifetime */
	return 0;
}
