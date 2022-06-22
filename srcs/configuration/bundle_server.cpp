#include "bundle_server.hpp"

Bundle_server::Bundle_server()
{
}

Bundle_server::Bundle_server(Server_conf server, unsigned short port) : _host(server.getHost()), _port(port)
{
	this->servers.push_back(server);
}

Bundle_server::Bundle_server(std::vector<Server_conf> servers, unsigned short port) : servers(servers), _host(servers[0].getHost()), _port(port)
{
}

void Bundle_server::addServer(Server_conf server, unsigned short port)
{
	if (this->servers.empty()) {
		this->_host = server.getHost();
		this->_port = port;
	}
	this->servers.push_back(server);
}

std::vector<Server_conf> Bundle_server::getServers() const
{
	return this->servers;
}

std::string Bundle_server::getHost() const
{
	return this->_host;
}

unsigned short Bundle_server::getPort() const
{
	return this->_port;
}

Bundle_server &Bundle_server::operator=(Bundle_server const &rhs)
{
	this->servers = rhs.servers;
	return *this;
}

Server_conf Bundle_server::get_server_from_server_name(std::string server_name)
{
	unsigned int iter = 0;

	if (this->servers.empty())
		throw std::invalid_argument("get_server_from_hostname: vector is empty");

	while (iter < this->servers.size())
	{
		if (this->servers[iter].name_is_in_list(server_name))
			return this->servers[iter];
		iter++;
	}
	return this->servers[0];
}

void Bundle_server::print_servers(void)
{
	unsigned int iter = 0;

	while (iter < this->servers.size())
	{
		this->servers[iter].shortprintServer();
		iter++;
	}
}
