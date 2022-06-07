#include "../webserv.hpp"

int	http_header_content_type( Request &req, Response &res )
{
	(void)req;
	std::string ext = get_extension(res.get_url());

	try {
		MimeType mi = mimes.getMimeForExtension(ext);
		if (!mi.needCGI()) {
			res.add_header("Content-Type", mi.getContentType());
		} else {
			res.add_header("Content-Type", mimes.getMimeForExtension("html").getContentType());
		}
	} catch(const MimeType::ExceptionUnknownMimeType &e) {
		res.add_header("Content-Type", "application/octet-stream");
	}

	return (1);
}