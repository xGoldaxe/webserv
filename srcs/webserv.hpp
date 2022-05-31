#ifndef WEBSERV_HPP
#	define WEBSERV_HPP

#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <sstream>

#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>

/* class */
class Response;
class Route;
struct webserv_conf;

#include "class/route.hpp"
#include "class/response.hpp"
#include "class/request.hpp"
#include "class/webserv_conf.hpp"

#include "http_header/http_header.hpp"

#include "internal/mime_types.hpp"


/* utils */
int							is_file(const char* name);
bool 						file_exist(const std::string& name);
bool						file_readable(const std::string &name);
std::string 				to_string( int number );
std::string 				read_binary( std::string filename );
std::string					get_extension(std::string file_name);
std::string					finish_by_only_one( std::string str, char c );
std::string					read_fd( int fd );
bool						usable_file( const std::string &name );
std::vector<std::string>	split_str( std::string str );

std::string                 ltrim(std::string s);
std::string                 rtrim(std::string s);
std::string                 trim(std::string s);

/* http */
int http_get_response( Request &req, Response &res );

#define PRINT_RES true

#endif
