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
	if (recv(client_socket, buffer, sizeof(buffer), MSG_PEEK | MSG_DONTWAIT) == 0)
	{
		std::cout << "Client close remote: " << client_socket << std::endl;
		close(client_socket);
		return;
	}

	/* we will need further verification */
	Webserv_conf conf;

	Request req(client_socket, conf);
	req.env = env;
	Response res(client_socket, conf, req);

	http_get_response(req, res);
}

void signalHandler(int signum)
{
	std::cout << std::endl
			  << "Goodbye! That was cool top have you :)" << std::endl;

	exit_code = signum;
}

int main(int argc, char **argv, char **env)
{
	if (argc == 2 && argv[1])
	{
		try
		{
			Webserv_conf conf = Webserv_conf(argv[1]);
		}
		catch (const std::exception &e)
		{
			std::cerr << e.what() << '\n';
		}
		return (0);
	}
	
	mimes.setDefault();

	Server serv = Server();
	try {
		serv.init_connection();
	} catch (const std::exception &e) {
		std::cerr << "Can't launch server!" << std::endl;
		std::cerr << e.what() << std::endl;
		return (1);
	}

	signal(SIGINT, signalHandler);

	exit_code = 0;

	while (exit_code == 0) {
		serv.handle_client();

		struct epoll_event evlist[SIZE];
		int nbr_req = epoll_wait(serv.get_poll_fd(), evlist, SIZE, 0);
		for (int i = 0; i < nbr_req; ++i)
		{
			std::cout << "read from, fd: " << evlist[i].data.fd << std::endl;
			process_request(evlist[i].data.fd, env);
		}
	}

	return (exit_code);
}
