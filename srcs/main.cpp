#include "init/server.hpp"

#include <strings.h>

#include "webserv.hpp"

#define SIZE 1024

void process_request(int client_socket, char **env)
{
	std::string req_raw_data;
	char buffer[256];
	/* with MSG_PEEK, no data will be ride of the socket */
	if ( recv(client_socket, buffer, sizeof(buffer), MSG_PEEK | MSG_DONTWAIT) == 0 )
	{
		std::cout << "Client close remote: " << client_socket << std::endl;
		close( client_socket );
		return ;
	}

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
	webserv_conf	conf; 
	
	Request req( req_raw_data, conf );
	req.env = env;
	Response res( client_socket, conf, req );

	// std::cout << "request url: " << req.getUrl() << std::endl;
	http_get_response(req, res);
}

int main(int argc, char **argv, char **env)
{
	(void) argc;
	(void) argv;
	(void) env;

	Server serv = Server();
	struct sockaddr_in cli_addr;

	///////////////////////////////
	int epfd = epoll_create1(O_CLOEXEC);
	fcntl(serv.get_socket(), F_SETFL, O_NONBLOCK);
	while(1) {
		// we accept some new request
		while (1)
		{
			socklen_t clilen = sizeof(cli_addr);
			int client_socket = accept(serv.get_socket(), (struct sockaddr *)&cli_addr, &clilen);
			if (client_socket == -1)
				break;

			int swtch = 1;	/* 1=KeepAlive On, 0=KeepAlive Off. */
			int idle = 7200;	/* Number of idle seconds before sending a KeepAlive probe. */
			int interval= 75;	/* How often in seconds to resend an unacked KeepAlive probe. */
			int count = 9;	/* How many times to resend a KA probe if previous probe was unacked. */

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
