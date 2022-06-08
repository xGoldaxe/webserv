#include "server.hpp"

Server_conf::Server_conf(void) : server_name(DEFAULT_SERVER_NAME), host(DEFAULT_HOST), body_max_size(DEFAULT_BODY_MAX_SIZE), root(DEFAULT_ROOT),
read_timeout(DEFAULT_READ_TIMEOUT), server_body_size(DEFAULT_SERVER_BODY_SIZE), client_header_size(DEFAULT_CLIENT_HEADER_SIZE),
max_amount_of_request(DEFAULT_MAX_AMOUNT_OF_REQUEST), max_uri_size(DEFAULT_MAX_URI_SIZE)
{
	this->port.push_back(3000);
	this->index.push_back("index.html");
	Route route1("/", "./www");
	Route route2("/php", "./cgi");
	route1.add_redirection(401,"/moved.html", "/sub/index.html");
	routes.push_back(route1);
	routes.push_back(route2);
	routes.back().set_enable_cgi(true);
	routes.back().set_cgi_path("/usr/bin/php");
	routes.back().add_cgi_extension("php");
	// routes.at(0).add_error_page(404, "defaultPages/404.html");
}

Server_conf::~Server_conf(void)
{
}

// empty
Server_conf::Server_conf(int emp) : server_name(DEFAULT_SERVER_NAME), host(DEFAULT_HOST), body_max_size(DEFAULT_BODY_MAX_SIZE), root(DEFAULT_ROOT), read_timeout(DEFAULT_READ_TIMEOUT), server_body_size(DEFAULT_SERVER_BODY_SIZE), client_header_size(DEFAULT_CLIENT_HEADER_SIZE), max_amount_of_request(DEFAULT_MAX_AMOUNT_OF_REQUEST), max_uri_size(DEFAULT_MAX_URI_SIZE)
{
	(void)emp;
}

int Server_conf::getReadTimeOut() const
{
	return this->read_timeout;
}
int Server_conf::getServerBodySize() const
{
	return this->server_body_size;
}
int Server_conf::getClientHeaderSize() const
{
	return this->client_header_size;
}

std::vector<short> Server_conf::getPort() const
{
	if (this->port.empty()) {
		return std::vector<short>(DEFAULT_PORT);
	}
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

std::string Server_conf::getHost() const
{
	return this->host;
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

void Server_conf::setHost(std::string host)
{
	this->host.clear();
	this->host.append(host);
}

void Server_conf::setRouteRoot(std::string root)
{
	this->routes.back().set_root(root);
}

void Server_conf::setReadTimeOut(int read_timeout)
{
	this->read_timeout = read_timeout;
}
void Server_conf::setServerBodySize(int server_body_size)
{
	this->server_body_size = server_body_size;
}
void Server_conf::setClientHeaderSize(int client_header_size)
{
	this->client_header_size = client_header_size;
}

void Server_conf::setRouteAutoIndex(bool auto_index)
{
	this->routes.back().set_auto_index(auto_index);
}

void Server_conf::set_cgi_timeout(int cgi_timeout)
{
	this->routes.back().set_cgi_timeout(cgi_timeout);
}
void Server_conf::set_send_file(bool send_file)
{
	this->routes.back().set_send_file(send_file);
}
void Server_conf::set_file_limit(int file_limit)
{
	this->routes.back().set_file_limit(file_limit);
}

void Server_conf::set_enable_cgi(bool enable_cgi)
{
	this->routes.back().set_enable_cgi(enable_cgi);
}

void Server_conf::addRouteRedirection(int redirect_code, std::string url, std::string redirect)
{
	this->routes.back().add_redirection(redirect_code, url, redirect);
}

void Server_conf::addRouteCGIExtension(std::string extension)
{
	this->routes.back().add_cgi_extension(extension);
}

void Server_conf::printServer()
{
#ifdef DEBUG

	std::vector<short>::iterator itp;
	std::vector<std::string>::iterator iti;
	std::map<int, std::string>::iterator ite;
	unsigned int itr = 0;

	std::cout << "********Server********" << std::endl;

	std::cout << "Server name : " << this->server_name << std::endl;
	std::cout << "Host : " << this->host << std::endl;
	std::cout << "Body Max Size : " << this->body_max_size << std::endl;
	std::cout << "root : " << this->root << std::endl;

	std::cout << "Read Timeout : " << this->read_timeout << std::endl;
	std::cout << "Server Body Size : " << this->server_body_size << std::endl;
	std::cout << "Client Header Size : " << this->client_header_size << std::endl;
	std::cout << "Max Amount of Requests : " << this->max_amount_of_request << std::endl;
	std::cout << "Max URI Size : " << this->max_uri_size << std::endl;

	if ((!this->port.empty()))
	{
		std::cout << "Ports : ";
		for (itp = this->port.begin(); itp != this->port.end(); itp++)
		{
			std::cout << *itp << " ";
		}
		std::cout << std::endl;
	}

	std::cout << "Index : ";

	if (this->index.empty())
		std::cout << "None";
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
	if (this->routes.empty())
		std::cout << "None";
	while (itr < this->routes.size())
	{
		this->routes[itr].printRoute();
		itr++;
	}
	std::cout << std::endl;
#endif
}

int Server_conf::get_max_amount_of_request() const
{
	return this->max_amount_of_request;
}

void Server_conf::set_max_amount_of_request(int max_amount_of_request)
{
	this->max_amount_of_request = max_amount_of_request;
}

int Server_conf::get_max_uri_size() const
{
	return this->max_uri_size;
}

void Server_conf::set_max_uri_size(int max_uri_size)
{
	this->max_uri_size = max_uri_size ;
}
