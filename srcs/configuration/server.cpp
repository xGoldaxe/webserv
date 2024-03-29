#include "server.hpp"

Server_conf::Server_conf(void) : host(DEFAULT_HOST), body_max_size(DEFAULT_BODY_MAX_SIZE), root(DEFAULT_ROOT),
								 onread_Timeout(DEFAULT_READ_TIMEOUT), server_body_size(DEFAULT_SERVER_BODY_SIZE), client_header_size(DEFAULT_CLIENT_HEADER_SIZE),
								 max_amount_of_request(DEFAULT_MAX_AMOUNT_OF_REQUEST), max_uri_size(DEFAULT_MAX_URI_SIZE),
								 run_file_path(DEFAULT_RUN_FILE_PATH), chunk_head_limit(DEFAULT_CHUNK_HEAD_LIMIT_SERVER), chunk_body_limit(DEFAULT_CHUNK_BODY_LIMIT_SERVER), idle_timeout(DEFAULT_IDLE_TIMEOUT_SERVER), process_data_size(DEFAULT_PROCESS_DATA_SIZE_SERVER)
{
	this->server_name.push_back(DEFAULT_SERVER_NAME);
	this->port.push_back(3000);
	this->index.push_back("index.html");
	Route route1("/", "./www");
	Route route2("/php", "./cgi");
	route1.add_redirection(401, "/moved.html", "/sub/index.html");
	routes.push_back(route1);
	routes.push_back(route2);
	routes.back().set_enable_cgi(true);
	routes.back().set_cgi_path("/usr/bin/php");
	routes.back().add_cgi_extension("php");
	// routes.at(0).add_error_page(404, "defaultPages/404.html");
}

Server_conf::Server_conf(const Server_conf &rhs) : port(rhs.port), routes(rhs.routes), server_name(rhs.server_name), host(rhs.host), index(rhs.index), body_max_size(rhs.body_max_size), root(rhs.root), error_pages(rhs.error_pages), onread_Timeout(rhs.onread_Timeout), server_body_size(rhs.server_body_size), client_header_size(rhs.client_header_size), max_amount_of_request(rhs.max_amount_of_request), max_uri_size(rhs.max_uri_size), run_file_path(rhs.run_file_path), chunk_head_limit(rhs.chunk_head_limit),
												   chunk_body_limit(rhs.chunk_body_limit), idle_timeout(rhs.idle_timeout), process_data_size(rhs.process_data_size)
{
}

Server_conf &Server_conf::operator=(const Server_conf &rhs)
{
	if (this != &rhs)
	{
		this->port = rhs.port;
		this->routes = rhs.routes;
		this->server_name = rhs.server_name;
		this->host = rhs.host;
		this->index = rhs.index;
		this->body_max_size = rhs.body_max_size;
		this->root = (rhs.root);
		this->error_pages = (rhs.error_pages);
		this->onread_Timeout = (rhs.onread_Timeout);
		this->server_body_size = (rhs.server_body_size);
		this->client_header_size = (rhs.client_header_size);
		this->max_amount_of_request = (rhs.max_amount_of_request);
		this->max_uri_size = (rhs.max_uri_size);
		this->run_file_path = (rhs.run_file_path);
		this->chunk_head_limit = (rhs.chunk_head_limit);
		this->chunk_body_limit = (rhs.chunk_body_limit);
		this->idle_timeout = (rhs.idle_timeout);
		this->process_data_size = rhs.process_data_size;
	}
	return (*this);
}

Server_conf::~Server_conf(void)
{
}

// empty
Server_conf::Server_conf(int emp) : host(DEFAULT_HOST), body_max_size(DEFAULT_BODY_MAX_SIZE),
									root(DEFAULT_ROOT), onread_Timeout(DEFAULT_READ_TIMEOUT), server_body_size(DEFAULT_SERVER_BODY_SIZE), client_header_size(DEFAULT_CLIENT_HEADER_SIZE), max_amount_of_request(DEFAULT_MAX_AMOUNT_OF_REQUEST),
									max_uri_size(DEFAULT_MAX_URI_SIZE), run_file_path(DEFAULT_RUN_FILE_PATH), chunk_head_limit(DEFAULT_CHUNK_HEAD_LIMIT_SERVER), chunk_body_limit(DEFAULT_CHUNK_BODY_LIMIT_SERVER), idle_timeout(DEFAULT_IDLE_TIMEOUT_SERVER), process_data_size(DEFAULT_PROCESS_DATA_SIZE_SERVER)
{
	(void)emp;
}

int Server_conf::getReadTimeOut() const
{
	return this->onread_Timeout;
}
int Server_conf::getIdleTimeOut() const
{
	return this->idle_timeout;
}
int Server_conf::getServerBodySize() const
{
	return this->server_body_size;
}
int Server_conf::getClientHeaderSize() const
{
	return this->client_header_size;
}

std::vector<unsigned short> Server_conf::getPort() const
{
	return this->port;
}
std::vector<Route> Server_conf::getRoutes() const
{
	return this->routes;
}
std::vector<std::string> Server_conf::getName() const
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

void Server_conf::resetPorts()
{
	this->port.clear();
}

void Server_conf::addPort(unsigned short port)
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
	this->server_name.push_back(server_name);
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

void Server_conf::setBodyMaxSizeRoute(int body_max_size)
{
	this->routes.back().setBodyMaxSize(body_max_size);
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

void Server_conf::setReadTimeOut(int onread_Timeout)
{
	this->onread_Timeout = onread_Timeout;
}
void Server_conf::setIdleTimeOut(int idle_timeout)
{
	this->idle_timeout = idle_timeout;
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

	std::vector<unsigned short>::iterator itp;
	std::vector<std::string>::iterator iti;
	std::map<int, std::string>::iterator ite;
	std::vector<std::string>::iterator its;
	std::vector<Route>::iterator itr;

	std::cout << "********Server********" << std::endl;

	std::cout << "Server name : ";
	if (this->server_name.empty())
		std::cout << "None";
	for (its = this->server_name.begin(); its != this->server_name.end(); its++)
	{
		std::cout << *its << " ";
	}
	std::cout << std::endl;

	std::cout << "Host : " << this->host << std::endl;
	std::cout << "Body Max Size : " << this->body_max_size << std::endl;
	std::cout << "root : " << this->root << std::endl;

	std::cout << "On Read Timeout : " << this->onread_Timeout << std::endl;
	std::cout << "Idle Timeout : " << this->idle_timeout << std::endl;
	std::cout << "Server Body Size : " << this->server_body_size << std::endl;
	std::cout << "Process Data Size : " << this->process_data_size << std::endl;
	std::cout << "Client Header Size : " << this->client_header_size << std::endl;
	std::cout << "Chunk Head Limit : " << this->chunk_head_limit << std::endl;
	std::cout << "Chunk Body Limit : " << this->chunk_body_limit << std::endl;

	std::cout << "Max Amount of Requests : " << this->max_amount_of_request << std::endl;
	std::cout << "Max URI Size : " << this->max_uri_size << std::endl;
	std::cout << "Run File Path: " << this->run_file_path << std::endl;

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
	itr = this->routes.begin();
	while (itr != this->routes.end())
	{
		(*itr).printRoute();
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
	this->max_uri_size = max_uri_size;
}

void Server_conf::set_cgi_path(std::string cgi_path)
{
	this->routes.back().set_cgi_path(cgi_path);
}

void Server_conf::check_methods_route(void)
{
	std::vector<Route>::iterator itr = this->routes.begin();
	while (itr != this->routes.end())
	{
		(*itr).check_methods_route();
		itr++;
	}
}

std::string Server_conf::getRunFilePath() const
{
	return this->run_file_path;
}

void Server_conf::setRunFilePath(std::string path)
{
	this->run_file_path.clear();
	this->run_file_path.append(path);
}

int Server_conf::getChunkHeadLimit() const
{
	return this->chunk_head_limit;
}
void Server_conf::setChunkHeadLimit(int chunk_head_limit)
{
	this->chunk_head_limit = chunk_head_limit;
}
int Server_conf::getChunkBodyLimit() const
{
	return this->chunk_body_limit;
}
void Server_conf::setChunkBodyLimit(int chunk_body_limit)
{
	this->chunk_body_limit = chunk_body_limit;
}

void Server_conf::setChunkHeadLimitRoute(int chunk_head_limit)
{
	this->routes.back().setChunkHeadLimit(chunk_head_limit);
}

void Server_conf::setChunkBodyLimitRoute(int chunk_body_limit)
{
	this->routes.back().setChunkBodyLimit(chunk_body_limit);
}

bool Server_conf::name_is_in_list(std::string server_name)
{
	std::vector<std::string>::iterator it = this->server_name.begin();

	while (it != this->server_name.end())
	{
		if ((*it).compare(server_name) == 0)
			return true;
		it++;
	}
	return false;
}

void Server_conf::sortPort()
{
	std::sort(this->port.begin(), this->port.end());
}

void Server_conf::setMultipartSizeRoute(int multipart_size)
{
	this->routes.back().set_max_multipart_size(multipart_size);
}
void Server_conf::setUploadSizeRoute(int upload_size)
{
	this->routes.back().set_max_upload_size(upload_size);
}

void Server_conf::shortprintServer()
{
#ifdef DEBUG

	std::vector<unsigned short>::iterator itp;
	std::vector<std::string>::iterator its;

	std::cout << "********Server********" << std::endl;

	std::cout << "Server name : ";
	if (this->server_name.empty())
		std::cout << "None";
	for (its = this->server_name.begin(); its != this->server_name.end(); its++)
	{
		std::cout << *its << " ";
	}
	std::cout << std::endl;

	std::cout << "Host : " << this->host << std::endl;
	std::cout << "Root : " << this->root << std::endl;

	if ((!this->port.empty()))
	{
		std::cout << "Ports : ";
		for (itp = this->port.begin(); itp != this->port.end(); itp++)
		{
			std::cout << *itp << " ";
		}
		std::cout << std::endl;
	}


	std::vector<Route>::iterator itr;
	std::cout << "Routes : ";
	std::cout << std::endl;
	if (this->routes.empty())
		std::cout << "None";
	itr = this->routes.begin();
	while (itr != this->routes.end())
	{
		std::cout << "Route root: " << (*itr).get_root() << std::endl;
		itr++;
	}
	std::cout << std::endl;

#endif
}

void Server_conf::set_process_data_size(std::size_t process_data_size)
{
	this->process_data_size = process_data_size;
}

std::size_t Server_conf::get_process_data_size()
{
	return process_data_size;
}