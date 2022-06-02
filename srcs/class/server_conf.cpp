#include "server_conf.hpp"

Server_conf::Server_conf(void)
{
	this->server_name = DEFAULT_SERVER_NAME;
	this->port.push_back(3000);
	this->index.push_back("index.html");
	Route route1("/", "./www");
	Route route2("/php", "./cgi");
	route1.add_redirection("/moved.html", "/sub/index.html");
	routes.push_back(route1);
	routes.push_back(route2);
	routes.back().enable_cgi("/usr/bin/php");
	routes.at(0).add_error_page(404, "defaultPages/404.html");
	this->body_max_size = 2048;
	this->root = DEFAULT_ROOT;
}

Server_conf::~Server_conf(void)
{
}

// empty
Server_conf::Server_conf(int emp)
{
	(void)emp;
	// default server name
	this->server_name = DEFAULT_SERVER_NAME;
	// default root
	this->root = DEFAULT_ROOT;
}

std::list<short> Server_conf::getPort() const
{
	return this->port;
}
std::vector<Route> Server_conf::getRoutes() const
{
	return this->routes;
}
std::string Server_conf::getName() const
{
	return this->server_name;
}

std::vector<std::string> Server_conf::getIndex() const
{
	return this->index;
}

int Server_conf::getBodyMaxSize() const
{
	return this->body_max_size;
}

std::string Server_conf::getRoot() const
{
	return this->root;
}

void Server_conf::addPort(short port)
{
	this->port.push_back(port);
}

void Server_conf::addMethods(std::string methods)
{
	this->routes.back().add_methods(methods);
}

void Server_conf::addRoute(Route route)
{
	this->routes.push_back(route);
}
void Server_conf::setName(std::string server_name)
{
	this->server_name.clear();
	this->server_name.append(server_name);
}
void Server_conf::addIndex(std::string index)
{
	this->index.push_back(index);
}

void Server_conf::addIndexRoute(std::string index)
{
	this->routes.back().add_index(index);
}

void Server_conf::addLastRouteErrorPages(int error, std::string errorpage)
{
	this->routes.back().add_error_page(error, errorpage);
}

void Server_conf::addErrorPages(int status_code, std::string error_message)
{
	std::pair<int, std::string> pair(status_code, error_message);
	this->error_pages.insert(pair);
}

void Server_conf::setBodyMaxSize(int body_max_size)
{
	this->body_max_size = body_max_size;
}

void Server_conf::setRoot(std::string root)
{
	this->root.clear();
	this->root.append(root);
}

void Server_conf::printServer()
{
#ifdef DEBUG

	std::list<short>::iterator itp;
	std::vector<std::string>::iterator iti;
	std::map<int, std::string>::iterator ite;
	unsigned int itr = 0;

	std::cout << "********Server********" << std::endl;

	std::cout << "Server name : " << this->server_name << std::endl;
	std::cout << "Body Max Size : " << this->body_max_size << std::endl;
	std::cout << "root : " << this->root << std::endl;

	if ((!this->port.empty()))
	{
		std::cout << "Ports : ";
		for (itp = this->port.begin(); itp != this->port.end(); itp++)
		{
			std::cout << *itp;
		}
		std::cout << std::endl;
	}

	std::cout << "Index : ";

	for (iti = this->index.begin(); iti != this->index.end(); iti++)
	{
		std::cout << *iti;
	}
	std::cout << std::endl;

	std::cout << "Error Pages : " << std::endl;
	for (ite = this->error_pages.begin(); ite != this->error_pages.end(); ite++)
	{
		std::cout << ite->first << " " << ite->second << std::endl;
	}
	std::cout << std::endl;

	std::cout << "Routes : ";
	std::cout << std::endl;
	while (itr < this->routes.size())
	{
		this->routes[itr].printRoute();
		itr++;
	}
	std::cout << std::endl;
#endif
}