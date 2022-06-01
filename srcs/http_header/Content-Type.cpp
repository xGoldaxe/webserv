#include "../webserv.hpp"

int	http_header_content_type( Request &req, Response &res )
{
	std::string ext = get_extension(req.getUrl());

	try {
		MimeType mi = mimes.getMimeForExtension(ext);
		res.add_header("Content-Type", mi.getContentType());
	} catch(MimeType::ExceptionUnknownMimeType *e) {
		res.add_header("Content-Type", "application/octet-stream");
	}

	return (1);
}