#pragma once

#include <vector>
#include <list>
#include <cstdlib>
#include <limits>
#include <fstream>
#include <sstream>
#include <algorithm>

#include "../class/route.hpp"
#include "server.hpp"
#include "bundle_server.hpp"

class Server_conf;

#define SIZE_PARSING 30

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
#define CGI_TIMEOUT_PARSING 13
#define READ_TIMEOUT_PARSING 14
#define SERVER_BODY_SIZE_PARSING 15
#define SEND_FILE_PARSING 16
#define FILE_LIMIT_PARSING 17
#define CLIENT_HEADER_SIZE_PARSING 18
#define HOST_PARSING 19
#define MAX_AMOUNT_OF_REQUEST_PARSING 20
#define MAX_URI_SIZE_PARSING 21
#define CGI_PATH_PARSING 22
#define RUN_FILE_PATH_PARSING 23
#define CHUNK_HEAD_LIMIT_PARSING 24
#define CHUNK_BODY_LIMIT_PARSING 25
#define IDLE_TIMEOUT_PARSING 26
#define MAX_MULTIPART_SIZE_PARSING 27
#define MAX_UPLOAD_SIZE_PARSING 28
#define PROCESS_DATA_SIZE_PARSING 29


class Webserv_conf
{
	private:
		std::vector<Server_conf>	servers;
		std::string 				http_version;
	public:
		Webserv_conf(void);
		Webserv_conf(std::string filename);
		~Webserv_conf(void);

		std::vector<Server_conf> 	getServers() const;
		std::string				 	getHttpVersion() const;

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

