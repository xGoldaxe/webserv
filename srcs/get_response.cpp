#include "webserv.hpp"
#include "errors/http_code.hpp"

bool verify_method( const Request &req, std::string method ) {
	return strcmp( req.getMethod().c_str(), method.c_str() ) == 0;
	return strcmp( req.getMethod().c_str(), method.c_str() ) == 0 && req.is_allowed_method(method);
}

int http_get_response( Request &req, Response &res ) {

	if ( res.status_code >= 400 ) /* if an error occure before executing the requst e.g invalid request */
	{
		res.error_body();
		res.send();
		close( res.client_socket );
		return 1;
	}

	req.try_url( res );
	/* generic headers */
	http_header_date(req, res);
	http_header_server(req, res);
	res.add_header("Connection", "Keep-Alive");
	res.add_header("Keep-Alive", "timeout=5, max=10000");
	res.send();

	return 1;
}