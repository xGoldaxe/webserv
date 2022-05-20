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

/* utils */
bool is_file(const char* name);
bool file_exist(const std::string& name);
std::string to_string( int number );

/* http */
int http_get_response( Request &req, Response &res );

#endif
