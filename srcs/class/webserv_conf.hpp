#ifndef WEBSERV_CONF_HPP
#	define WEBSERV_CONF_HPP

#include <iostream>
#include <string>
#include <vector>
#include "route.hpp"
#include "../webserv.hpp"

struct webserv_conf
{
	std::string 				root;
	std::vector<std::string>	index;
	std::string 				http_version;
	std::string					server_name;
	std::vector<Route>			routes;

	webserv_conf(void) {

		root = ".";
		index.push_back("index.html");
		http_version = "HTTP/1.1";
		server_name = "webserv (42) v0.1-dev";
		Route route1( "/", "./www" );
		Route route2( "/php", "./cgi" );
		routes.push_back( route1 );
		routes.push_back( route2 );
		routes.back().enable_cgi( "/usr/bin/php" );
	};
	webserv_conf( webserv_conf &rhs ) : root(rhs.root) {};
	~webserv_conf(void) {};
};


#endif