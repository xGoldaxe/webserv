#pragma once

#include <string>
#include <list>
#include <vector>
#include <map>
#include <iostream>

#include "../class/route.hpp"

#define DEFAULT_SERVER_NAME "webserv.bg"
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
#define DEFAULT_RUN_FILE_PATH "/run/webserv/"
#define DEFAULT_CHUNK_HEAD_LIMIT_SERVER 20
#define DEFAULT_CHUNK_BODY_LIMIT_SERVER 100
#define DEFAULT_IDLE_TIMEOUT_SERVER 60

class Route;

class Server_conf
{

	private:
		std::vector<unsigned short>			port;
		std::vector<Route>					routes;
		std::vector<std::string>			server_name;
		std::string							host;
		std::vector<std::string>			index;
		int									body_max_size;
		std::string 						root;
		std::map<int, std::string>			error_pages;
		int									onread_Timeout;
		int									server_body_size;
		int									client_header_size;
		int									max_amount_of_request;
		int									max_uri_size;
		std::string							run_file_path;
		int									chunk_head_limit;
		int									chunk_body_limit;
		int									idle_timeout;

	public:
		Server_conf(void);
		~Server_conf(void);
		Server_conf(const Server_conf &rhs);
		Server_conf(int emp);
		Server_conf& operator=(const Server_conf& other);



		void 							setBodyMaxSizeRoute(int body_max_size);
		int								getReadTimeOut() const;
		int								getIdleTimeOut() const;
		int								getServerBodySize() const;
		int								getClientHeaderSize() const;
		std::vector<unsigned short>		getPort() const;
		std::vector<Route>				getRoutes() const;
		std::string						getHost() const;
		std::vector<std::string>		getName() const;
		std::vector<std::string>		getIndex() const;
		int								getBodyMaxSize() const;
		std::string						getRoot() const;
		void							addPort(unsigned short port);
		void							resetPorts();
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
		void							setReadTimeOut(int onread_Timeout);
		void							setIdleTimeOut(int idle_timeout);
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
		void 							check_methods_route(void);
		std::string						getRunFilePath() const;
		void							setRunFilePath(std::string path);
		int								getChunkHeadLimit() const;
		void							setChunkHeadLimit(int chunk_head_limit);
		int								getChunkBodyLimit() const;
		void							setChunkBodyLimit(int chunk_body_limit);
		void							setChunkHeadLimitRoute(int chunk_body_limit);
		void							setChunkBodyLimitRoute(int chunk_body_limit);
		void 							shortprintServer();
		bool							name_is_in_list(std::string server_name);
		void 							sortPort();

};
