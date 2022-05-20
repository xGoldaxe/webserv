#ifndef WEBSERV_CONF_HPP
#	define WEBSERV_CONF_HPP

#include <iostream>
#include <string>
#include <vector>

struct webserv_conf
{
	std::string root;
	std::vector<std::string> index;

	webserv_conf(void) {

		root = ".";
		index.push_back("index.html");
	};
	webserv_conf( webserv_conf &rhs ) : root(rhs.root) {};
	~webserv_conf(void) {};
};


#endif