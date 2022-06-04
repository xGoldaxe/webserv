#include "../webserv.hpp"

int	http_header_content_length( const Request &req, Response &res ) {
	(void) req;
	if (res.body.size() > 1024) {
		res.add_header("Transfer-Encoding", "chunked");
	} else {
		res.add_header("Content-Length", to_string(res.body.size()));
	}
	return (1);
}