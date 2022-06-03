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

#include "../internal/mime_types.hpp"
#include "../class/request.hpp"

#define WEBSERV_VERSION "webserv-1.0"

class CGIManager {
public:
    typedef std::vector<std::string> t_headers_list;
    typedef std::map<std::string, std::string> t_header_value;
    typedef std::vector<MimeType> t_mime_list;

    CGIManager(t_mime_list mimes, std::string cgi_path);
    ~CGIManager();
    std::string    exec(Request &req);

private:
    std::vector<MimeType>   _mime_types;
    t_header_value          _headers;
    std::vector<char *>     _c_headers;
    std::string             _cgi_path;

    void addHeader(std::string name, std::string value);
    void computeEnvArray();
};
