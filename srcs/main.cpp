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

void process_request(int client_socket)
{
	webserv_conf	conf; 
	conf.root = "./www";

	std::string req_raw_data;
	std::string res;
	char buffer[256];
	bzero(buffer,256);
	req_raw_data = "";
	int n = 255;
	while (n == 255)
	{
		n = read(client_socket, buffer, 255);
		req_raw_data += buffer;
	}
	Request req(req_raw_data, conf);

	std::cout << "request url: " << req.getUrl() << std::endl;
	http_get_response(req, res); 
	send(client_socket, res.c_str(), res.size(), 0);
}

int main(void)
{
	int server_socket = socket(
		AF_INET,
		SOCK_STREAM,
		0
	);

	short port = 3000;

	struct sockaddr_in	server_addr;
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
	int epfd = epoll_create1(0);
	fcntl(server_socket, F_SETFL, O_NONBLOCK);
	while(1) {
		// we accept some new request
		while (1)
		{
			client_socket = accept(server_socket, NULL, NULL);
			if (client_socket == -1)
				break;
			struct epoll_event ev;
			ev.events = EPOLLIN;
			ev.data.fd = client_socket;
			epoll_ctl(epfd, EPOLL_CTL_ADD, client_socket, &ev);
			// std::cout << "new registered connection!" << std::endl;
		}

		struct epoll_event evlist[1024];
		int nbr_req = epoll_wait(epfd, evlist, 1024, 0);
		for (int i = 0; i < nbr_req; ++i)
		{
			process_request( evlist[i].data.fd );
			close( evlist[i].data.fd );
		}
	}
	/* connections must have a lifetime */
	return 0;
}
