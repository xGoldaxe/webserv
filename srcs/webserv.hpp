#ifndef WEBSERV_HPP
#	define WEBSERV_HPP

#include <iostream>
#include <string>
#include <map>
#include <sstream>
#include <filesystem>

#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "class/webserv_conf.hpp"
#include "class/request.hpp"
#include "class/response.hpp"

#include "http_header/http_header.hpp"

/* utils */
bool is_file(const char* name);
bool file_exist(const std::string& name);
std::string to_string( int number );
std::string	finish_by_only_one( std::string str, char c );
std::string read_binary( std::string filename );
std::string get_extension(std::string file_name);

/* http */
int http_get_response( Request &req, Response &res );

#endif
