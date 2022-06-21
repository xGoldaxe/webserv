#include "response.hpp"

// trim from start
inline static std::string ltrim(std::string s)
{
	s.erase(s.begin(), std::find_if(s.begin(), s.end(),
									std::not1(std::ptr_fun<int, int>(std::isspace))));
	return s;
}

// trim from end
inline static std::string rtrim(std::string s)
{
	s.erase(std::find_if(s.rbegin(), s.rend(),
						 std::not1(std::ptr_fun<int, int>(std::isspace)))
				.base(),
			s.end());
	return s;
}

// trim from end
inline static std::string trim(std::string s)
{
	return ltrim(rtrim(s));
}

Response::Response(void)
{
}

Response::Response(int client_socket, std::vector<std::string> index, Request *req, const char *client_ip, size_t max_size)
	: _return_body_type(BODY_TYPE_STRING),
	  _client_ip(client_ip),
	  _body_max_size(max_size),
	  _route(req->get_route()),
	  _index(index),
	  _is_custom_error(false),
	  _body_file(req->get_body_file()),
	  _cgi(NULL),
	  _loaded_body( false ),
	  auto_index( false ),
	  posted_files( req->get_posted_files() ),
	  client_socket(client_socket),
	  req(req)
{
	// this->_add_file( body_file );
	this->version = "HTTP/1.1";
	if (this->req->is_request_valid())
	{
		std::string path = this->req->get_legacy_url().substr(this->_route.get_location().size());

		if (path.find_first_of('/') == 0) {
			this->url = this->_route.get_root() + path.substr(1);
		} else {
			this->url = this->_route.get_root() + path;
		}
	}
	else
	{
		this->set_status( this->req->get_status().first, this->req->get_status().second );
	}
}

Response::Response( Response const &src )
{
	*this = src;
	std::cout << "FORBIDDEN CALL!" << std::endl;
}

Response &   Response::operator=( Response const & rhs )
{
	this->_index = rhs._index;
	this->version = rhs.version;
	this->headers = rhs.headers;
	this->req = rhs.req;
	this->_cgi = NULL;

	this->status_code = rhs.status_code;
	this->status_message = rhs.status_message;
	this->_body_max_size = rhs._body_max_size;
	this->body = rhs.body;
	this->client_socket = rhs.client_socket;

	this->_return_body_type = rhs._return_body_type;
	this->_file_len = rhs._file_len;
	this->url = rhs.url;

	return *this;
}

Response::~Response(void)
{
	if ( this->req != NULL )
		delete this->req;
	if (this->_cgi != NULL)
		delete this->_cgi;
}

void Response::output(const size_t req_id)
{
	#ifdef DEBUG
		std::cout << "[" << this->version << "]";
	#endif
	std::cout << "[http://" << this->req->get_header_value("Host") << "]";
	std::cout << "[" << this->_client_ip << "]";
	std::cout << "[" << this->get_str_code() << "]";
	std::cout << "[" << req_id << "]";
	#ifdef DEBUG
		std::cout << "[" << this->_body_max_size << "]";
	#endif
	std::cout << " " << this->req->getMethod();
	std::cout << " " << this->req->get_legacy_url() << std::endl;
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

/* getters */
std::string Response::get_url(void)
{
	return this->url;
}

size_t Response::getFileSize(void)
{
	return this->_file_len;
}

int Response::send()
{
	// In the case of a CGI Execution, we don't want to manage headers/body assets
	if (this->_return_body_type == BODY_TYPE_CGI)
	{
		return (1);
	}

	/* add some headers */
	http_header_content_length(*this->req, *this);

	std::string headers_response = this->version + " " + to_string(this->status_code) + " " + this->status_message;
	for (headers_t::iterator it = this->headers.begin(); it != this->headers.end(); ++it)
		headers_response += "\n" + it->first + ": " + it->second;
	headers_response += "\r\n\r\n";

	if (this->req->getMethod() == "HEAD")
	{
		// headers_response += "\r\n";
	}
	else if (this->_return_body_type == BODY_TYPE_STRING && this->body.length() < this->_body_max_size)
	{
		headers_response += this->body + "\r\n";
	}

	int status = ::send(this->client_socket, headers_response.c_str(), headers_response.length(), 0);
	if (this->req->getMethod() == "HEAD" || !(this->body.length() > this->getChunkMaxSize() || this->isFile()))
	{
		return (-1);
	}
	return (status);
}

size_t	Response::get_size_next_chunk()
{
	if (this->_return_body_type == BODY_TYPE_FILE) {
		return std::min<size_t>(this->_body_max_size, this->_file_len);
	}
	return std::min<size_t>(this->_body_max_size, this->body.length());
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
		std::string response_body(this->body, 0, std::min<size_t>(this->_body_max_size, this->body.length()));
		std::string response_content = intToHex(response_body.length()) + "\r\n" + response_body + "\r\n";
		this->body.erase(0, this->_body_max_size);
		::send(this->client_socket, response_content.c_str(), response_content.length(), 0);
		return this->body.length();
	}
	else if (this->_return_body_type == BODY_TYPE_CGI)
	{
		int chunk_type = 0;

		try {
			chunk_type = this->_cgi->readChunk(this->_body_max_size);
		} catch (const HTTPCode500 &e) {
			this->set_status( e.getCode(), e.getDescription() );
			this->error_body();
			this->send();
			return this->send();
		} catch (const HTTPCode5XX &e) {
			this->set_status( e.getCode(), e.getDescription() );
			this->error_body();
			this->send();
			return this->send_chunk();
		}

		if (chunk_type == CHUNK_CONTINUE) {
			std::string out = this->_cgi->getOutput();
			if (out.length() > 0) {
				std::string response_content = intToHex(out.length()) + "\r\n" + out + "\r\n";
				::send(this->client_socket, response_content.c_str(), response_content.length(), 0);	
			}
			return (1);
		} else if (chunk_type == CHUNK_HEADER) {
			std::string headers_response = this->version + " ";

			std::string headers = this->_cgi->getOutput();
			size_t pos_status = headers.find("Status: ");
			if (pos_status == headers.npos) {
				headers_response += to_string(this->status_code) + " " + this->status_message + "\r\n";
			} else {
				std::string header_status = headers.substr(pos_status + std::string("Status: ").length());
				headers_response += header_status.substr(0, header_status.find_first_of("\r\n", pos_status)) + "\r\n";
				std::cout << headers_response << std::endl;
				headers.erase(pos_status, headers.find_first_of("\r\n", pos_status) + 2);
			}

			headers_response += headers;

			::send(this->client_socket, headers_response.c_str(), headers_response.length(), 0);
			return (headers_response.length());
		}

		if (chunk_type == CHUNK_NEXT)
			return (1);
		return (0);
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

		::send(this->client_socket, response_content.c_str(), response_content.length(), 0);

		this->_file_len -= transmit_size;

		if (this->_file_len <= 0 && this->_in_file.is_open())
			this->_in_file.close();
		return this->_file_len;
	}
	else
	{
		::send(this->client_socket, this->body.c_str(), this->body.length(), 0);
		return 0;
	}
}

bool Response::isFile()
{
	return this->_return_body_type == BODY_TYPE_FILE;
}

bool Response::isCGI()
{
	return this->_return_body_type == BODY_TYPE_CGI;
}

std::string auto_index_template(std::string url, std::string legacy_url);

std::string Response::load_body(std::string client_ip)
{
	if ( this->_loaded_body == true )
		return this->body;
	this->_loaded_body = true;
	if (this->auto_index) {
		this->add_header("Content-Type", "text/html");
		this->body = auto_index_template( this->url, this->req->get_legacy_url() );
	}
	else if (this->_route.get_cgi_enable() && this->_route.is_in_extension(get_extension(this->url.c_str())))
	{
		this->_return_body_type = BODY_TYPE_CGI;
		
		try {
			this->_cgi = new CGIManager(this->_route.get_root(), this->_route.get_cgi_path(), this->url, this->_route.get_cgi_timeout());
			this->body = this->_cgi->exec(*this->req, client_ip);
		} catch (const HTTPError &e) {
			this->set_status( e.getCode(), e.getDescription() );
			this->error_body();
			return this->body;
		}

		std::istringstream resp(this->body);
		std::string header;
		while (std::getline(resp, header) && header != "\r") {
			std::string::size_type index = header.find(':');
			std::string name = trim(header.substr(0, index));
			std::string value = trim(header.substr(index + 1));

			if (name == "Status") {
				index = value.find(' ');
				this->set_status(atoi(value.substr(0, index).c_str()), value.substr(index + 1));
			} else if (index != std::string::npos) {
				this->add_header(name, value);
			}
		}
		this->add_header("Content-Type", "text/html");
		this->add_header("Transfer-Encoding", "chunked");

		std::stringstream tmp;
		tmp << resp.rdbuf();
		this->body = header + tmp.str();

	}
	else
	{
		this->_return_body_type = BODY_TYPE_FILE;
		if (this->_is_custom_error)
			this->_in_file.open(this->body.c_str(), std::ios::binary);
		else
			this->_in_file.open(this->url.c_str(), std::ios::binary);
		if (this->_in_file.fail())
			throw HTTPCode404();

		this->_in_file.seekg(0, this->_in_file.end);
		this->_file_len = this->_in_file.tellg();
		this->_in_file.seekg(0, this->_in_file.beg);
	}
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

std::string Response::_template_body( const std::string & legacy_url, const std::string & message )
{
	if ( this->_loaded_body )
		return this->body;
	this->_loaded_body = true;
	this->add_header("Content-Type", "text/html");
	this->body = std::string( std::string("<!DOCTYPE html>\
		<html lang=\"en\">\
		<head>\
			<meta charset=\"UTF-8\">\
			<meta http-equiv=\"X-UA-Compatible\" content=\"IE=edge\">\
			<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\
		</head>\
		<body>\
			<h1>") + legacy_url + " " + message + std::string("</h1>\
		</body>\
		</html>") );
	return this->body;
}
/* ************************************************************************** */
/*                                                                            */
/*            @FROM REQUEST TO RESPONSE                                       */
/*                                                                            */
/* ************************************************************************** */

std::string	store_cat_test( bool mode, std::string value = std::string() ) {

	static std::string value_stored = std::string();

	if ( mode == true )
		value_stored = value;
	return (value_stored);
}

bool	cat_test( std::string it, std::string &res )
{
	res = store_cat_test( false ) + it;
	return ( is_file( res ) == IS_FILE_NOT_FOLDER );
}

std::string go_through_it_until(std::vector<std::string> values,
	bool (*rule)(std::string, std::string &))
{
	std::string res;
	for (std::vector<std::string>::iterator it = values.begin(); it != values.end(); ++it)
	{
		if (rule(*it, res))
			return res;
	}
	return "";
}

void	Response::check_file_url(void)
{
	this->auto_index = false;
	if ( is_file( this->url ) == IS_FILE_NOT_FOLDER  )
	{
		if ( !file_readable( this->url ) )
			throw HTTPCode403();
		return ;
	}
	else if ( is_file( this->url ) == IS_FILE_FOLDER )
	{
		if ( this->_route.get_auto_index() == true )
		{
			this->auto_index = true;
			return ;
		}
		std::vector<std::string> indexes = this->_route.get_index();
		for (std::vector<std::string>::iterator it = indexes.begin(); it != indexes.end(); it++) {
			if (is_file( this->url + *it ) == IS_FILE_NOT_FOLDER) {
				this->req->auto_index = false;
				this->url = this->url + *it;
				return;
			}
		}
	}
	throw HTTPCode404();
}

/* this function is use to route the url, and test many case ( redirection, cache ... )
and if no conditions are check it goes to check the file to serve and throw an error if its fail */
/* each case work as block that can be interchanged ( except the last one ) */
/* nous on a que deux cas a gerer, redirection et la fallback */

void Response::try_url(std::string client_ip) {
	// Try any redirection
	try {
		Redirection redir = this->_route.return_redirect_url(this->req->get_legacy_url());
		this->set_status( redir.get_redirect_code(), "Moved Permanently" );
		this->add_header( "Location", redir.get_redirect_url() );
		return ;
	} catch (const std::out_of_range &e) {}

	// Try to reach local file
	try
	{
		this->check_file_url();
		// here we route the methods
		if ( this->req->getMethod() == "DELETE" )
		{
			if ( is_file( this->url ) != IS_FILE_NOT_FOLDER )
				HTTPCode403();
			int status = remove( this->url.c_str() );
    		if ( status != 0 )
				HTTPCode403();
			this->_template_body( this->req->get_legacy_url(), "deleted sucessfully!");
		}
		else if ( this->req->getMethod() == "POST" )
		{
			std::string subpath = delete_filename( this->get_url() ) + "/";
			for ( std::vector< std::pair<std::string, std::string> >::iterator it = this->posted_files.begin();
			it != this->posted_files.end(); ++it )
			{
				std::string path = subpath + it->first;
				if ( is_file( path ) != IS_FILE_NOT_FOLDER )
					throw HTTPCode403();
				std::ofstream File( path.c_str() );
				if ( File.is_open() == false )
					throw HTTPCode403();

				File << it->second;
			}
		}
		this->set_status( 200, "OK" );
		this->load_body(client_ip); ////
		http_header_content_type( *this->req, *this );
	}
	catch (const HTTPError &e) {
		this->set_status( e.getCode(), e.getDescription() );
		this->error_body();
	}
}
size_t Response::getChunkMaxSize()
{
	return this->_body_max_size;
}