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

#include "webserv_conf.hpp"
#include "class/request.hpp"

/* utils */
bool is_file(const char* name);
bool file_exist(const std::string& name);


/* http */
int http_get_response( Request &req, std::string &res );

#endif
