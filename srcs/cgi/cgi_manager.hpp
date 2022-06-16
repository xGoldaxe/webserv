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
#include <poll.h>

#include "internal/mime_types.hpp"
#include "class/request.hpp"

#define WEBSERV_VERSION "webserv-1.0"
#define CHUNK_CONTINUE 1
#define CHUNK_NEXT 2
#define CHUNK_OVER 3
#define CHUNK_HEADER 4

class Request;

class CGIManager {
public:
    typedef std::vector<std::string> t_headers_list;
    typedef std::map<std::string, std::string> t_header_value;

    CGIManager(std::string root, std::string cgi_path, std::string path, int cgi_timeout);
    ~CGIManager();
    std::string    exec(Request &req, std::string client_ip);
    int            readChunk(std::size_t chunk_size);
    std::string    getOutput();

private:
    t_header_value          _headers;
    char                    **_c_headers;
    std::string             _cgi_path;
    std::string             _path;
    std::string             _remaining;
    std::string             _out_chunk;
    int                     _cgi_timeout;
    int                     _cgi_out_fd;
    pid_t                   _pid;
    bool                    _sent_headers;
    std::time_t             _begin_response_time;

    void addHeader(std::string name, std::string value);
    void computeEnvArray();
    void cleanCHeaders();
};
