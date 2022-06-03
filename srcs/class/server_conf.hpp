#pragma once

#include "route.hpp"
#include <list>
#include <vector>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cstring>
#include <limits>
#include <cstdlib>
#include <map>
#include <iostream>
#include <string>

class Route;

#define DEFAULT_SERVER_NAME "webserv (42) v0.1-dev"
#define DEFAULT_ROOT "."

class Server_conf
{

	private:
		std::list<short>					port;
		std::vector<Route>					routes;
		std::string							server_name;
		std::vector<std::string>			index;
		int									body_max_size;
		std::string 						root;
		std::map<int, std::string>			error_pages;

	public:
		Server_conf(void);
		~Server_conf(void);
		Server_conf(int emp);

		std::list<short>				getPort() const;
		std::vector<Route>				getRoutes() const;
		std::string						getName() const;
		std::vector<std::string>		getIndex() const;
		int								getBodyMaxSize() const;
		std::string						getRoot() const;
		void							addPort(short port);
		void 							addRoute(Route route);
		void 							setName(std::string server_name);
		void							addIndex(std::string index);
		void							addIndexRoute(std::string index);
		void 							addMethods(std::string methods);
		void 							addErrorPages(int error, std::string errorpage);
		void 							addLastRouteErrorPages(int error, std::string errorpage);
		void							setBodyMaxSize(int body_max_size);
		void							setRoot(std::string root);
		void							setRouteRoot(std::string root);
		void							printServer();



};
