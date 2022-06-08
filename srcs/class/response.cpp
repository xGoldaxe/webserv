#include "response.hpp"

Response::Response(void)
{}

Response::Response(int client_socket, Webserv_conf conf, Request const *req, const char *client_ip, size_t max_size) : conf(conf), req(req), _return_body_type(BODY_TYPE_STRING), _client_ip(client_ip), _body_max_size(max_size), status_code(-1), client_socket(client_socket)
{
	this->cpy_req = *(this->req);
	this->version = "HTTP/1.1";
}

Response::Response( Response const &src )
{
	*this = src;
	std::cout << "FORBIDDEN CALL!" << std::endl;
}

Response &   Response::operator=( Response const & rhs )
{
	this->conf = rhs.conf;
	this->version = rhs.version;
	this->headers = rhs.headers;
	this->req = rhs.req;
	this->cpy_req = *(this->req);

	this->status_code = rhs.status_code;
	this->status_message = rhs.status_message;
	this->_body_max_size = rhs._body_max_size;
	this->body = rhs.body;
	this->client_socket = rhs.client_socket;

	this->_return_body_type = rhs._return_body_type;
	this->_file_len = rhs._file_len;

	return *this;
}

Response::~Response(void)
{}

void Response::output()
{
	std::cout << "[" << this->version << "]";
	std::cout << "[http://" << this->cpy_req.get_header_value("Host") << "]";
	std::cout << "[" << this->_client_ip << "]";
	std::cout << "[" << this->get_str_code() << "]";
	std::cout << "[" << this->_body_max_size << "]";
	std::cout << " " << this->cpy_req.getMethod();
	std::cout << " " << this->cpy_req.get_legacy_url() << std::endl;
}

std::string Response::get_str_code(void)
{
	return to_string(this->status_code);
}

int Response::add_header(std::string key, std::string value)
{
	this->headers.insert(headers_t::value_type(key, value));
	return (1);
}

void Response::set_status(int status_code, std::string msg)
{
	this->status_code = status_code;
	this->status_message = msg;
}

int Response::send()
{
	/* add some headers */
	http_header_content_length(*this->req, *this);

	std::string headers_response = this->version + " " + to_string(this->status_code) + " " + this->status_message;
	for (headers_t::iterator it = this->headers.begin(); it != this->headers.end(); ++it)
		headers_response += "\n" + it->first + ": " + it->second;
	headers_response += "\r\n\n";

	if (this->_return_body_type == BODY_TYPE_STRING && this->body.size() < this->_body_max_size) {
		headers_response += this->body + "\r\n";
	}

	int status = ::send(this->client_socket, headers_response.c_str(), headers_response.size(), 0);
	return (status);
}

size_t	Response::get_size_next_chunk()
{
	if (this->_return_body_type == BODY_TYPE_FILE) {
		return std::min<size_t>(this->_body_max_size, this->_file_len);
	}
	return std::min<size_t>(this->_body_max_size, this->body.size());
}

/**
 * Send the current chunk, and move to the next one
 * 
 * @return -1 if an error occured
 * @return 0 if the file is completely read
 * @return x the remaining length
 */
int		Response::send_chunk()
{
	if (this->_return_body_type == BODY_TYPE_STRING)
	{
		std::string response_body(this->body, 0, std::min<size_t>(this->_body_max_size, this->body.size()));
		std::string response_content = intToHex(response_body.size()) + "\r\n" + response_body + "\r\n";
		this->body.erase(0, this->_body_max_size);
		::send(this->client_socket, response_content.c_str(), response_content.size(), 0);
		return this->body.size();
	}
	else if (this->_return_body_type == BODY_TYPE_FILE)
	{
		if (!this->_in_file.is_open())
			return -1;

		char buf[this->_body_max_size + 1];
		memset(buf, 0, this->_body_max_size + 1);

		size_t transmit_size = this->get_size_next_chunk();
		this->_in_file.read(buf, transmit_size);

		std::string response_content = intToHex(transmit_size) + "\r\n" + std::string(buf, transmit_size) + "\r\n";

		::send(this->client_socket, response_content.c_str(), response_content.size(), 0);

		this->_file_len -= transmit_size;

		if (this->_file_len <= 0 && this->_in_file.is_open())
			this->_in_file.close();
		return this->_file_len;
	}
	else
	{
		::send(this->client_socket, this->body.c_str(), this->body.size(), 0);
		return 0;
	}
}

bool Response::isFile()
{
	return this->_return_body_type == BODY_TYPE_FILE;
}

std::string auto_index_template(std::string url, std::string legacy_url);

std::string Response::load_body( Request &req )
{
	if (req.auto_index) {
		this->add_header("Content-Type", "text/html");
		this->body = auto_index_template( req.getUrl(), req.get_legacy_url() );
	} else if (req.get_route().get_cgi_enable() && req.get_route().is_in_extension(get_extension(req.getUrl()))) {
		CGIManager cgi(req.get_route().get_cgi_path(), "/home/restray/42/webserv/tests-42");
		this->body = cgi.exec(req);
		this->add_header("Content-Type", "text/html");
	} else {
		try {
			this->_return_body_type = BODY_TYPE_FILE;
			this->_in_file.open(req.getUrl().c_str(), std::ios::binary);

			this->_in_file.seekg(0, this->_in_file.end);
			this->_file_len = this->_in_file.tellg();
			this->_in_file.seekg(0, this->_in_file.beg);
		} catch (const std::exception &e) {
			std::cerr << e.what() << std::endl;
			/** @todo On peut renvoyer une erreur 404 ici! */
		}
	}
	this->cpy_req = req;
	return this->body;
}

std::string auto_index_template(std::string url, std::string legacy_url)
{
	std::string files;
	/* get each files */

	DIR *d;
	struct dirent *dir;
	d = opendir(url.c_str());
	if (d)
	{
		while ((dir = readdir(d)) != NULL)
		{
			std::string dirname = dir->d_name;
			if (is_file(std::string(finish_by_only_one(url, '/') + dirname).c_str()) == 0)
				dirname = finish_by_only_one(dirname, '/');
			if (dirname != "./")
				files += "<p><a href=\"" + finish_by_only_one(legacy_url, '/') + dirname + "\">" + dirname + "</a></p>";
		}
		closedir(d);
	}
	return std::string(std::string("<!DOCTYPE html>\
		<html lang=\"en\">\
		<head>\
			<meta charset=\"UTF-8\">\
			<meta http-equiv=\"X-UA-Compatible\" content=\"IE=edge\">\
			<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\
		</head>\
		<body>\
			<h1>Index of ") +
					   legacy_url + std::string("</h1>\
			<hr />\
			<p>") + files +
					   std::string("</p>\
		<hr />\
		</body>\
		</html>"));
}

const Webserv_conf &Response::get_conf() const
{
	return this->conf;
}

size_t Response::getChunkMaxSize()
{
	return this->_body_max_size;
}
