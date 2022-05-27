#ifndef WEBSERV_CONF_HPP
#	define WEBSERV_CONF_HPP


#include <vector>
#include <string>
#include "route.hpp"

struct webserv_conf
{
	std::string 				root;
	std::vector<std::string>	index;
	std::string 				http_version;
	std::string					server_name;
	std::vector<Route>			routes;

	webserv_conf(void);
	webserv_conf( webserv_conf &rhs );
	~webserv_conf(void) {};
};


#endif