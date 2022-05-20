#ifndef WEBSERV_CONF_HPP
#	define WEBSERV_CONF_HPP

#include <iostream>
#include <string>
#include <vector>

struct webserv_conf
{
	std::string 				root;
	std::vector<std::string>	index;
	std::string 				http_version;
	std::string					server_name;

	webserv_conf(void) {

		root = ".";
		index.push_back("index.html");
		http_version = "HTTP/1.1";
		server_name = "webserv (42) v0.1-dev";
		std::cout << "Server name: " << server_name << std::endl;
	};
	webserv_conf( webserv_conf &rhs ) : root(rhs.root) {};
	~webserv_conf(void) {};
};


#endif