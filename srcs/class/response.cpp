#include "response.hpp"

Response::Response(int client_socket, Webserv_conf &conf, Request const &req) : conf(conf), req(req), _return_body_type(BODY_TYPE_STRING), status_code(-1), client_socket(client_socket)
{
	this->version = this->conf.getHttpVersion();

	if (req.is_request_valid() == false)
		this->set_status(400, "Bad Request");
	else if (this->version != req.get_http_version())
		this->set_status(505, "HTTP Version Not Supported");
}

Response::~Response(void)
{
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
	http_header_content_length(this->req, *this);

	std::string raw_response;

	raw_response = this->version + " " + to_string(this->status_code) + " " + this->status_message;
	for (headers_t::iterator it = this->headers.begin(); it != this->headers.end(); ++it)
		raw_response += "\n" + it->first + ": " + it->second;
	raw_response += "\r\n\n";

	#ifdef PRINT_REQ
		std::cout << "<====>" << std::endl;
		std::cout << raw_response << std::endl;
		std::cout << "<====>" << std::endl;
	#endif

	int status = ::send(this->client_socket, raw_response.c_str(), raw_response.size(), 0);

	if (this->body.size() > MAX_BODY_LENGTH || this->_return_body_type == BODY_TYPE_FILE)
	{
		if (this->_return_body_type == BODY_TYPE_STRING) {
			while (status > 0 && this->body.size() > 0)
			{
				std::string response_body(this->body, 0, std::min<size_t>(MAX_BODY_LENGTH, this->body.size()));
				std::string response_content = intToHex(response_body.size()) + "\r\n" + response_body + "\r\n";
				status = ::send(this->client_socket, response_content.c_str(), response_content.size(), 0);
				this->body.erase(0, MAX_BODY_LENGTH);
			}
		} else if (this->_return_body_type == BODY_TYPE_FILE) {
			std::ifstream in_file(this->body.c_str(), std::ios::binary);

			in_file.seekg(0, in_file.end);
			size_t length = in_file.tellg();
			in_file.seekg(0, in_file.beg);

			char buf[MAX_BODY_LENGTH + 2 + 1];
			memset(buf, 0, MAX_BODY_LENGTH + 2 + 1);
			while (length > 0 && status > 0 && in_file && exit_code == 0)
			{
				size_t transmit_size = std::min<size_t>(MAX_BODY_LENGTH, length);
				in_file.read(buf, transmit_size);

				std::string response_content = intToHex(transmit_size) + "\r\n" + std::string(buf, transmit_size) + "\r\n";
			
				status = ::send(this->client_socket, response_content.c_str(), response_content.size(), 0);

				/* with MSG_PEEK, no data will be ride of the socket */
				char buffer[256];
				if (recv(this->client_socket, buffer, sizeof(buffer), MSG_PEEK | MSG_DONTWAIT) == 0)
				{
					std::cout << "Client close remote: " << this->client_socket << std::endl;
					close(this->client_socket);
					break;
				}

				memset(buf, 0, MAX_BODY_LENGTH + 2 + 1);
				length -= transmit_size;
			}
			std::cerr << "quit" << std::endl;

			if (in_file.is_open())
				in_file.close();
		} else {
			throw HTTPCode500();
		}

		if (status >= 0) {
			std::string response_content = "0\r\n\r\n";
			status = ::send(this->client_socket, response_content.c_str(), response_content.size(), 0);
		}
	} else {
		status = ::send(this->client_socket, this->body.c_str(), this->body.size(), 0);
	}

	return (status);
}

bool Response::isFile()
{
	return this->_return_body_type == BODY_TYPE_FILE;
}

std::string auto_index_template(std::string url, std::string legacy_url);

/**
 * @todo free all string_to_char
 */
std::string Response::load_body( Request &req )
{
	if (req.auto_index) {
		this->add_header("Content-Type", "text/html");
		this->body = auto_index_template( req.getUrl(), req.get_legacy_url() );
	} else if (req.get_route().get_cgi_enable() && get_extension( req.getUrl().c_str() ) == req.get_route().get_cgi_extension()) {
		CGIManager cgi(req.get_route().get_cgi_path(), "/home/restray/42/webserv/tests-42");
		this->body = cgi.exec(req);
		this->add_header("Content-Type", "text/html");
	} else {
		try {
			this->_return_body_type = BODY_TYPE_FILE;
			this->body = req.getUrl();
		} catch (const std::exception &e) {
			std::cerr << e.what() << std::endl;
			/** @todo On peut renvoyer une erreur 404 ici! */
		}
	}
	return this->body;
}

std::string error_template(std::string error_code, std::string message) {

	return std::string(std::string("<!DOCTYPE html>\
	<html lang=\"en\">\
	<head>\
		<meta charset=\"UTF-8\">\
		<meta http-equiv=\"X-UA-Compatible\" content=\"IE=edge\">\
		<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\
		<title>Webserv - ") +
					   error_code + std::string("</title>\
		\
		<style>\
		p, h1 {\
			text-align: center;\
		}\
		</style>\
	</head>\
	<body>\
		<h1>WEBSERV ERROR - ") +
					   error_code + std::string("</h1>\
		<hr />\
		<p>") + message +
					   std::string("</p>\
	</body>\
	</html>"));
}

std::string & Response::error_body(void) {

	this->add_header("Content-Type", "text/html");
	try
	{
		//this line throw an error if page not find
		std::string filename = this->req.route.get_error_pages().at( this->status_code );
		if ( usable_file( filename ) )
		{
			this->_return_body_type = BODY_TYPE_FILE;
			this->body = filename;
		}
		else
		{
			this->set_status( 500, "Internal Server Error" );
			this->body = error_template(this->get_str_code(), this->status_message);
		}
	}
	catch(const std::exception& e)
	{
		this->body = error_template(this->get_str_code(), this->status_message);
	}
	
	return (this->body);
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
			p, h1 {\
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
