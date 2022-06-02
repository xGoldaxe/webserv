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

#include "internal/mime_types.hpp"

#include "class/route.hpp"
#include "class/response.hpp"
#include "class/request.hpp"
#include "class/webserv_conf.hpp"
#include "cgi/cgi_manager.hpp"

#include "http_header/http_header.hpp"

#include "go_through_until.hpp"

extern MimeTypes mimes;

/* utils */
#define IS_FILE_FOLDER 0
#define IS_FILE_NOT_FOLDER 1
#define IS_FILE_ERROR -1 //doesnt exist
int							is_file( std::string dir );
bool 						file_exist(const std::string& name);
bool						file_readable(const std::string &name);
std::string 				to_string( int number );
std::string 				read_binary( std::string filename );
std::string					get_extension(std::string file_name);
std::string					finish_by_only_one( std::string str, char c );
std::string					read_fd( int fd );
bool						usable_file( const std::string &name );
std::vector<std::string>	split_str( std::string str );

/* http */
int http_get_response( Request &req, Response &res );

#define PRINT_RES true

#endif
