#pragma once

#include <vector>
#include <fstream>
#include <sstream>
#include "route.hpp"
#include <list>
#include <algorithm>
#include <cstring>
#include <limits>
#include <cstdlib>
#include "server_conf.hpp"

class Server_conf;

#define SIZE_PARSING 13

#define SERVER_NAME_PARSING 0
#define LISTEN_PARSING 1
#define ERROR_PAGE_PARSING 2
#define LOCATION_PARSING 3
#define ROOT_PARSING 4
#define INDEX_PARSING 5
#define METHODS_PARSING 6
#define ENABLE_CGI_PARSING 7
#define CGI_EXTENSION_PARSING 8
#define BODY_MAX_SIZE_PARSING 9
#define SERVER_PARSING 10
#define REWRITE_PARSING 11
#define	AUTOINDEX_PARSING 12

class Webserv_conf
{
	private:
		std::vector<Server_conf>	servers;
		std::string 				http_version;
	public:
		Webserv_conf(void);
		Webserv_conf(std::string filename);
		std::vector<Server_conf> getServers() const;
		std::string				 getHttpVersion() const;

		Webserv_conf &   operator=( Webserv_conf const & rhs );

/* exceptions */
        class FailedToOpenFile : public std::exception	{
            public:
                virtual const	char* what() const throw()	{
                    return ("Webserv_conf::Webserv_conf Couldn't open file");
                }
        };
        class OutOfRangePort : public std::exception	{
            public:
                virtual const	char* what() const throw()	{
                    return ("Webserv_conf::Webserv_conf Provided Port is outside range!");
                }
        };
};

