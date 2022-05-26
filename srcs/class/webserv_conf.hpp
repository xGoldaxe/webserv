#ifndef WEBSERV_CONF_HPP
#	define WEBSERV_CONF_HPP

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include "route.hpp"
#include <list>
#include <algorithm>
#include <cstring>
#include "server_conf.hpp"

#define SERVER_NAME 0
#define LISTEN 1
#define ERROR_PAGE 2
#define LOCATION 3
#define ROOT 4
#define INDEX 5
#define METHODS 6
#define ENABLE_CGI 7
#define CGI_EXTENSION 8
#define BODY_MAX_SIZE 9

struct Webserv_conf
{
	std::string 				root;
	std::vector<Server_conf>	servers;
	std::string 				http_version;

	Webserv_conf(void);
	Webserv_conf(std::string filename);

//	void	print_conf(void);

/* exceptions */
        class FailedToOpenFile : public std::exception	{
            public:
                virtual const	char* what() const throw()	{
                    return ("Webserv_conf::Webserv_conf Couldn't open file");
                }
        };

};


#endif