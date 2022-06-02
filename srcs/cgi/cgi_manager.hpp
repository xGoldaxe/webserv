#pragma once

#include <vector>
#include <utility>
#include <map>
#include <string>
#include "../internal/mime_types.hpp"
#include "../class/request.hpp"

#define WEBSERV_VERSION "webserv-1.0"

class CGIManager {
    public:
    typedef std::vector<std::string> t_headers_list;
    typedef std::map<std::string, std::string> t_header_value;
    typedef std::vector<MimeType> t_mime_list;
    
    CGIManager(t_mime_list mimes);
    void    setRequiredHeadersName(void);
    void    fillServerInfos();
    void    exec(Request &req);

    private:
    std::vector<MimeType>   _mime_types;
    t_header_value          _headers;
};