#include "../webserv.hpp"

int	http_header_content_length( const Request &req, Response &res ) {
	(void) req;
	if (req.getMethod() == "HEAD") {
		if (res.isFile()) {
			res.add_header("Content-Length", to_string(res.getFileSize()));
		} else {
			res.add_header("Content-Length", to_string(res.body.length()));
		}
	} else if (res.body.length() > res.getChunkMaxSize() || res.isFile()) {
		res.add_header("Transfer-Encoding", "chunked");
	} else {
		res.add_header("Content-Length", to_string(res.body.length()));
	}
	return (1);
}