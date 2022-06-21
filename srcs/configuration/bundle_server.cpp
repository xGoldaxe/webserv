#include "bundle_server.hpp"

Bundle_server::Bundle_server()
{
}

Bundle_server::Bundle_server(std::vector<Server_conf> servers) : servers(servers)
{
}

void Bundle_server::addServer(Server_conf server)
{
	this->servers.push_back(server);
}

std::vector<Server_conf> Bundle_server::getServers() const
{
	return this->servers;
}

Bundle_server &Bundle_server::operator=( Bundle_server const & rhs ){
	this->servers = rhs.servers;
	return *this;
}

Server_conf Bundle_server::get_server_from_hostname(std::string hostname)
{
	unsigned int iter = 0;

	if(this->servers.empty())
		throw std::invalid_argument("get_server_from_hostname: vector is empty");

	while (iter < this->servers.size())
	{
		if (this->servers[iter].getHost().compare(hostname) == 0)
			return this->servers[iter];
		iter++;
	}
	return this->servers[0];
}

void	Bundle_server::print_servers(void)
{
	unsigned int iter = 0;

	while (iter < this->servers.size())
	{
		this->servers[iter].shortprintServer();
		iter++;
	}
}





