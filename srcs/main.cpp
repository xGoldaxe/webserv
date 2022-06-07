#include "webserv.hpp"
#include "init/server.hpp"

#define SIZE 1024

MimeTypes mimes;
int exit_code;

void signalHandler(int signum)
{
	std::cout << std::endl
			  << "Goodbye! That was cool to have you :)" << std::endl;

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
	signal(SIGPIPE, SIG_IGN);

	exit_code = 0;

	(void)env; /** @todo ajouter a la classe serveur */
	while (exit_code == 0) {
		serv.handle_client();
		serv.wait_for_connections();
		serv.trigger_queue();
		// serv.handle_responses();
	}

	return (exit_code);
}
