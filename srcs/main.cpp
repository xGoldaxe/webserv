#include "webserv.hpp"

MimeTypes mimes;

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

	std::vector<Server *> servers;
	std::vector<Server_conf> servers_config = conf.getServers();
	std::vector<Bundle_server> bundles = pack_servers(servers_config);
	
	for (std::vector<Bundle_server>::iterator it = bundles.begin(); it != bundles.end(); it++)
	{
		try
		{
			servers.push_back(new Server(env, *it));
			servers.back()->init_connection();
		}
		catch (const std::exception &e)
		{
			std::cerr << "Can't launch server!" << std::endl;
			std::cerr << e.what() << std::endl;
			return (1);
		}
	}

	handleExit();

	while (!shouldQuit())
	{
		for (std::vector<Server *>::iterator it = servers.begin(); it != servers.end() && !shouldQuit(); it++)
		{
			(*it)->handle_client();
			(*it)->wait_for_connections();
			(*it)->trigger_queue();
			(*it)->handle_responses();
		}
	}

	for (std::vector<Server *>::iterator it = servers.begin(); it != servers.end(); it++)
		delete (*it);

	return (exit_code);
}
