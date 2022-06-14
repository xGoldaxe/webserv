#pragma once

#include <string>
#include <list>
#include <vector>
#include <map>
#include <iostream>

#include "class/route.hpp"

#define DEFAULT_SERVER_NAME "webserv (42) v0.1-dev"
#define DEFAULT_ROOT "."
#define DEFAULT_BODY_MAX_SIZE 2048
#define DEFAULT_PORT 3000
#define DEFAULT_READ_TIMEOUT -1
#define DEFAULT_MAX_AMOUNT_OF_REQUEST 10
#define DEFAULT_SERVER_BODY_SIZE 2048
#define DEFAULT_CLIENT_HEADER_SIZE 2048
#define DEFAULT_HOST "127.0.0.1"
#define DEFAULT_INDEX_SERVER "index.html"
#define DEFAULT_MAX_URI_SIZE 50


class Route;

class Server_conf
{

	private:
		std::vector<unsigned short>			port;
		std::vector<Route>					routes;
		std::string							server_name;
		std::string							host;
		std::vector<std::string>			index;
		int									body_max_size;
		std::string 						root;
		std::map<int, std::string>			error_pages;
		int									read_timeout;
		int									server_body_size;
		int									client_header_size;
		int									max_amount_of_request;
		int									max_uri_size;

	public:
		Server_conf(void);
		~Server_conf(void);
		Server_conf(int emp);


		int								getReadTimeOut() const;
		int								getServerBodySize() const;
		int								getClientHeaderSize() const;
		std::vector<unsigned short>		getPort() const;
		std::vector<Route>				getRoutes() const;
		std::string						getHost() const;
		std::string						getName() const;
		std::vector<std::string>		getIndex() const;
		int								getBodyMaxSize() const;
		std::string						getRoot() const;
		void							addPort(unsigned short port);
		void 							addRoute(Route route);
		void 							setName(std::string server_name);
		void							addIndex(std::string index);
		void							addIndexRoute(std::string index);
		void 							addMethods(std::string methods);
		void 							addErrorPages(int error, std::string errorpage);
		void 							addLastRouteErrorPages(int error, std::string errorpage);
		void							setBodyMaxSize(int body_max_size);
		void							setRoot(std::string root);
		void							setHost(std::string host);
		void							setRouteRoot(std::string root);
		void							printServer();
		void							setReadTimeOut(int read_timeout);
		void							setServerBodySize(int server_body_size);
		void 							setClientHeaderSize(int client_header_size);
		void 							setRouteAutoIndex(bool auto_index);
		void							set_cgi_timeout(int cgi_timeout);
		void							set_cgi_path(std::string cgi_path);
		void							set_send_file(bool send_file);
		void							set_file_limit(int file_limit);
		void							set_enable_cgi(bool enable_cgi);
		void							addRouteRedirection(int redirect_code, std::string url, std::string redirect);
		void 							addRouteCGIExtension(std::string extension);
		int								get_max_amount_of_request() const;
		void							set_max_amount_of_request(int max_amount_of_request);
		int								get_max_uri_size() const;
		void							set_max_uri_size(int max_uri_size);
};
