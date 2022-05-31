#include "init/server.hpp"

#include <strings.h>

#include "webserv.hpp"

#define SIZE 1024

/* throw a server exeption in case of failure */

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
	while ( n == 255 )
	{
		bzero(buffer,256);
		n = read(client_socket, buffer, 255);
		buffer[n] = '\0';
		req_raw_data += buffer;
	}
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

	MimeTypes mimes;
	mimes.setDefault();

	/************************************************************************
	 * Example of working mimes parsing                                     */
	std::cout << mimes.getMimeForExtension("html") << std::endl;
	try {
		std::cout << mimes.getMimeForExtension("inconnu") << std::endl;
	} catch (std::exception *e) {
		std::cout << e->what() << std::endl;
	}

	mimes.parseHTTP("Content-Type = text/html");
	mimes.parseHTTP("Content-Type = text/html; charset=utf-8");
	mimes.parseHTTP("Content-Type = text/html ;Charset=utf-8");
	mimes.parseHTTP("Content-Type = text/html ; charset=utf-8");

	/* End of Example                                                       *
	*************************************************************************/

	Server serv = Server();
	serv.init_connection();

	while (true) {
		serv.handle_client();

		struct epoll_event evlist[1024];
		int nbr_req = epoll_wait(serv.get_poll_fd(), evlist, 1024, 0);
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
