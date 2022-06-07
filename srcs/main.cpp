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
	// Define the config file to use
	std::string config_filename("config/default.wbserv");
	if (argc == 2 && argv[1]) {
		config_filename = argv[1];
	} else {
		std::cerr << "[WARNING] Using " << config_filename << " as config file. Use the following command to set the used file\n./webserv config.wbserv" << std::endl;
	}

	// Create a the configuration parser class
	Webserv_conf conf;
	try
	{
		conf = Webserv_conf(config_filename);
	}
	catch (const std::exception &e)
	{
		std::cerr << e.what() << std::endl;
		return (1);
	}

	// Fill mimes
	mimes.setDefault();

	std::vector<Server> servers;
	std::vector<Server_conf> servers_config = conf.getServers();

	for (std::vector<Server_conf>::iterator it = servers_config.begin(); it != servers_config.end(); it++)
	{
		servers.push_back(Server(env, *it));
		try
		{
			servers.back().init_connection();
		}
		catch (const std::exception &e)
		{
			std::cerr << "Can't launch server!" << std::endl;
			std::cerr << e.what() << std::endl;
			return (1);
		}
	}

	exit_code = 0;
	signal(SIGINT, signalHandler);
	signal(SIGPIPE, SIG_IGN);

	while (exit_code == 0)
	{
		for (std::vector<Server>::iterator it = servers.begin(); it != servers.end(); it++)
		{
			it->handle_client();
			it->wait_for_connections();
			it->trigger_queue();
			it->handle_responses();
		}
	}

	return (exit_code);
}
