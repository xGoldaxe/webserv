#include "bundle_server_management.hpp"

std::vector<Bundle_server> pack_servers(std::vector<Server_conf> servers)
{
	std::map<std::pair<std::string, unsigned short>, Bundle_server> bundle_servers;

	for (std::vector<Server_conf>::iterator it = servers.begin(); it != servers.end(); it++)
	{
		std::vector<unsigned short> server_ports = it->getPort();
		for (std::vector<unsigned short>::iterator port_it = server_ports.begin(); port_it != server_ports.end(); port_it++)
		{
			Server_conf serv(*it);
			serv.resetPorts();
			serv.addPort(*port_it);
			bundle_servers[std::make_pair(it->getHost(), *port_it)].addServer(serv, *port_it);
		}
	}

	std::vector<Bundle_server> vector_servers;
	for (std::map<std::pair<std::string, unsigned short>, Bundle_server>::iterator it = bundle_servers.begin(); it != bundle_servers.end(); it++) {
		vector_servers.push_back(it->second);
	}
	
	return vector_servers;
}

void print_bundled_servers(std::vector<Bundle_server> bundle_servers)
{
	for (std::vector<Bundle_server>::iterator it = bundle_servers.begin(); it != bundle_servers.end(); it++)
	{
		std::cout << "************ Vector " << it->getHost() << ":" << it->getPort() << " *************" << std::endl;
		it->print_servers();
	}
}

Bundle_server	 get_bundle_server_from_hostname(std::vector<Bundle_server> bundle_servers, std::string hostname)
{
	unsigned int i = 0;

	if(bundle_servers.empty())
		throw std::invalid_argument("get_bundle_server_from_hostname empty vector");

	while (i < bundle_servers.size())
	{
		if(bundle_servers[i].getHost().compare(hostname) == 0)
			return bundle_servers[i];
		i++;
	}
	return bundle_servers[0];
}

Bundle_server	 get_bundle_server_from_hostname_and_port(std::vector<Bundle_server> bundle_servers, std::string hostname, unsigned short port)
{
	unsigned int i = 0;

	if(bundle_servers.empty())
		throw std::invalid_argument("get_bundle_server_from_hostname empty vector");

	while (i < bundle_servers.size())
	{
		if(bundle_servers[i].getHost().compare(hostname) == 0 && bundle_servers[i].getPort() == port)
			return bundle_servers[i];
		i++;
	}
	return bundle_servers[0];
}