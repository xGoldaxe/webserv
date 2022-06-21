#include "bundle_server_management.hpp"

std::vector<Bundle_server> pack_servers(std::vector<Server_conf> servers)
{
	std::vector<Bundle_server> bundle_servers;
	unsigned int jterator = 0;
	for (unsigned int i = 0; i < servers.size(); i++)
	{
		while (jterator < bundle_servers.size())
		{
			if (!bundle_servers[jterator].getServers().empty()
				&& bundle_servers[jterator].getServers()[0].getHost() == servers[i].getHost()
				&& bundle_servers[jterator].getServers()[0].getPort() == servers[i].getPort())
			{
				bundle_servers[jterator].addServer(Server_conf(servers[i]));
				break;
			}
			jterator++;
		}
		if (jterator == bundle_servers.size())
		{
			bundle_servers.push_back(Bundle_server(Server_conf(servers[i])));
		}
		
		jterator = 0;
	}
	return bundle_servers;
}

void print_bundled_servers(std::vector<Bundle_server> bundle_servers)
{
	for (unsigned int i = 0; i < bundle_servers.size(); i++)
	{
		std::cout << "************Vector " << i << " *************" << std::endl;
		bundle_servers[i].print_servers();
	}
}