#pragma once

#include <sys/types.h>
#include <stdlib.h>

#include "internal/mime_types.hpp"

#include "class/route.hpp"
#include "class/response.hpp"
#include "class/request.hpp"
#include "configuration/webserv.hpp"
#include "cgi/cgi_manager.hpp"

#include "http_header/http_header.hpp"

extern MimeTypes mimes;

/* http */
int http_get_response( Request &req, Response &res );
