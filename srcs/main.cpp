#include "webserv.hpp"
#include "init/server.hpp"

#define SIZE 1024

MimeTypes mimes;
int exit_code;

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
	std::cout << std::endl
			  << "Goodbye! That was cool top have you :)" << std::endl;

	exit_code = signum;
}

//testing confparser if you type ./webserv testconf
//to remove when done
void testconf()
{
	try {
		Webserv_conf conf = Webserv_conf("idOnotExist");
	} catch (const std::exception &e) {
		std::cerr << e.what() << std::endl;
	}

	try {
		Webserv_conf conf = Webserv_conf("./config/default.wbserv");
		conf.getServers()[0].printServer();
	} catch (const std::exception &e) {
		std::cerr << e.what() << std::endl;
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

	signal(SIGINT, signalHandler);

	Server serv = Server();
	try
	{
		serv.init_connection();
	} catch (const std::exception &e) {
		std::cerr << "Can't launch server!" << std::endl;
		std::cerr << e.what() << std::endl;
		return (1);
	}

	exit_code = 0;

	while (exit_code == 0) {
		serv.handle_client();

		struct epoll_event evlist[SIZE];
		int nbr_req = epoll_wait(serv.get_poll_fd(), evlist, SIZE, 0);
		for (int i = 0; i < nbr_req; ++i)
		{
			std::cout << "read from, fd: " << evlist[i].data.fd << std::endl;
			process_request( evlist[i].data.fd, env );
		}
	}

	return (exit_code);
}
