#pragma once

#include "internal/mime_types.hpp"

#include "class/route.hpp"
#include "class/request.hpp"
#include "class/response.hpp"
#include "init/server.hpp"
#include "init/connection.hpp"
#include "configuration/webserv.hpp"
#include "cgi/cgi_manager.hpp"

#include "http_header/http_header.hpp"

extern MimeTypes mimes;
extern int exit_code;

/* http */
int http_get_response( Request &req, Response &res );
