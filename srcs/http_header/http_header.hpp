#pragma once

#include "class/request.hpp"
#include "class/response.hpp"

class Request;
class Response;

int http_header_date(Request &req, Response &res);
int	http_header_server( Request &req, Response &res );
int	http_header_content_length( const Request &req, Response &res );
int	http_header_content_type( Request &req, Response &res );
