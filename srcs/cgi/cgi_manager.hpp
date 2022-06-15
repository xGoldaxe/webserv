#pragma once

#include <vector>
#include <utility>
#include <map>
#include <string>

#include <sys/types.h>
#include <cstdlib>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "internal/mime_types.hpp"
#include "class/request.hpp"

#define WEBSERV_VERSION "webserv-1.0"

class Request;

class CGIManager {
public:
    typedef std::vector<std::string> t_headers_list;
    typedef std::map<std::string, std::string> t_header_value;

    CGIManager(std::string root, std::string cgi_path, std::string path, int cgi_timeout);
    ~CGIManager();
    std::string    exec(Request &req, std::string client_ip);

private:
    t_header_value          _headers;
    char                    **_c_headers;
    std::string             _cgi_path;
    std::string             _path;
    int                     _cgi_timeout;

    void addHeader(std::string name, std::string value);
    void computeEnvArray();
    void cleanCHeaders();
};
