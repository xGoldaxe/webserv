#include "init/server.hpp"

#include <strings.h>

#include "webserv.hpp"

#define SIZE 1024

MimeTypes mimes;

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

	/* we will need further verification */
	Webserv_conf	conf; 
	
	Request req( client_socket, conf );
	req.env = env;
	Response res( client_socket, conf, req );

	http_get_response(req, res);
}

void signalHandler(int signum)
{
	std::cout << "Interrupt signal (" << signum << ") received.\n";

	// cleanup and close up stuff here
	// terminate program

	exit(signum);
}

//testing confparser if you type ./webserv testconf
//to remove when done
void testconf()
{
	
	try
	{
		Webserv_conf conf = Webserv_conf("idOnotExist");
	}
	catch (const std::exception &e)
	{
		std::cerr << e.what() << '\n';
	}

	try
	{
		Webserv_conf conf = Webserv_conf("./config/default.wbserv");
		conf.getServers()[0].printServer();
	}
	catch (const std::exception &e)
	{
		std::cerr << e.what() << '\n';
	}

}

int main(int argc, char **argv, char **env)
{
	(void) argc;
	(void) argv;

	if(argc == 2 && strcmp(argv[1],"testconf") == 0)
	{
		testconf();
		return(0);
	}

	mimes.setDefault();

	/************************************************************************
	 * Example of working mimes parsing                                     */
	std::cout << mimes.getMimeForExtension("html") << std::endl;
	try {
		std::cout << mimes.getMimeForExtension("inconnu") << std::endl;
	} catch (const MimeType::ExceptionUnknownMimeType &e) {
		std::cout << e.what() << std::endl;
	}

	mimes.parseHTTP("Content-Type = text/html");
	mimes.parseHTTP("Content-Type = text/html; charset=utf-8");
	mimes.parseHTTP("Content-Type = text/html ;Charset=utf-8");
	mimes.parseHTTP("Content-Type = text/html ; charset=utf-8");

	/* End of Example                                                       *
	*************************************************************************/

	/*************************************************************************
	 * Example CGI Manager usage											 */
	// std::vector<MimeType> cgi_mimes;
	// cgi_mimes.push_back(mimes.getMimeForExtension("php"));
	// CGIManager cgi(cgi_mimes);
	/* End of CGI Example                                                    *
	*************************************************************************/

	signal(SIGINT, signalHandler);

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
		}
	}

	/* connections must have a lifetime */
	return 0;
}
