#include "response.hpp"

Response::Response(void)
{
}

Response::Response(int client_socket, Webserv_conf conf, Request const req) : conf(conf), req(req), _return_body_type(BODY_TYPE_STRING), status_code(-1), client_socket(client_socket)
{
	this->cpy_req = this->req;
	this->version = "HTTP/1.1";
	if ( req.is_request_valid() )
	{
		this->url = this->req.route.get_root() + 
			req.get_legacy_url().substr(req.get_legacy_url().find_first_of(this->req.route.get_location())
			+ this->req.route.get_location().size());
	}
	this->set_status( req.get_status().first, req.get_status().second );
}

Response::Response( Response const &src )
{
	*this = src;
}

Response &   Response::operator=( Response const & rhs )
{
	this->conf = rhs.conf;
	this->version = rhs.version;
	this->headers = rhs.headers;
	this->req = rhs.req;
	this->cpy_req = this->req;

	this->status_code = rhs.status_code;
	this->status_message = rhs.status_message;
	this->body = rhs.body;
	this->client_socket = rhs.client_socket;

	this->_return_body_type = rhs._return_body_type;
	this->_file_len = rhs._file_len;

	return *this;
}

Response::~Response(void)
{
	std::cout << "[" << this->version << "][" << this->cpy_req.getMethod() << "][" << this->get_str_code() << "] " << this->cpy_req.get_legacy_url() << std::endl;
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

int Response::send()
{
	/* add some headers */
	http_header_content_length(this->req, *this);

	std::string headers_response = this->version + " " + to_string(this->status_code) + " " + this->status_message;
	for (headers_t::iterator it = this->headers.begin(); it != this->headers.end(); ++it)
		headers_response += "\n" + it->first + ": " + it->second;
	headers_response += "\r\n\n";

	if (this->_return_body_type == BODY_TYPE_STRING && this->body.size() < MAX_BODY_LENGTH) {
		headers_response += this->body + "\r\n";
	}

	int status = ::send(this->client_socket, headers_response.c_str(), headers_response.size(), 0);
	return (status);
}

size_t	Response::get_size_next_chunk()
{
	if (this->_return_body_type == BODY_TYPE_FILE) {
		return std::min<size_t>(MAX_BODY_LENGTH, this->_file_len);
	}
	return std::min<size_t>(MAX_BODY_LENGTH, this->body.size());
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
		std::string response_body(this->body, 0, std::min<size_t>(MAX_BODY_LENGTH, this->body.size()));
		std::string response_content = intToHex(response_body.size()) + "\r\n" + response_body + "\r\n";
		this->body.erase(0, MAX_BODY_LENGTH);
		::send(this->client_socket, response_content.c_str(), response_content.size(), 0);
		return this->body.size();
	}
	else if (this->_return_body_type == BODY_TYPE_FILE)
	{
		if (!this->_in_file.is_open())
			return -1;

		char buf[MAX_BODY_LENGTH + 1];
		memset(buf, 0, MAX_BODY_LENGTH + 1);

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
		this->body = auto_index_template( this->url, req.get_legacy_url() );
	} else if (req.get_route().get_cgi_enable() && get_extension( this->url.c_str() ) == req.get_route().get_cgi_extension()) {
		CGIManager cgi(req.get_route().get_cgi_path(), "/home/restray/42/webserv/tests-42");
		this->body = cgi.exec(req);
		this->add_header("Content-Type", "text/html");
	} else {
		try {
			this->_return_body_type = BODY_TYPE_FILE;
			this->_in_file.open(this->url.c_str(), std::ios::binary);

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
	throw HTTPCode404();
}

void	Response::check_file_url(void)
{
	// this->route.auto_index = false; /** @todo NEED TO DO THIS! */
	if ( /* this->route.auto_index && */ is_file( this->url ) == IS_FILE_FOLDER )
		this->req.auto_index = true;
	else if ( is_file( this->url ) == IS_FILE_NOT_FOLDER )
	{
		if ( !file_readable( this->url ) )
			throw HTTPCode403();
	}
	else
	{
		store_cat_test( true, finish_by_only_one( this->url, '/' ) );
		this->url = go_through_it_until(
			this->conf.getServers()[0].getIndex(), /** @todo NEED TO DO THIS! */
			&cat_test
		);
	}
}

bool	Response::is_redirection( std::string &redir_str ) {

	try
	{
		redir_str = this->req.route.get_redirections().at( 
			this->req.get_legacy_url() );
		return true;
	}
	catch(const std::exception& e)
	{
		return false;
	}
}

/* this function is use to route the url, and test many case ( redirection, cache ... )
and if no conditions are check it goes to check the file to serve and throw an error if its fail */
/* each case work as block that can be interchanged ( except the last one ) */
/* nous on a que deux cas a gerer, redirection et la fallback */

void Response::try_url() {

	try
	{
		std::string redir_str;
		if ( is_redirection( redir_str ) ) {
			this->set_status( 301, "Moved Permanently" );
			this->add_header( "Location", redir_str );
			return ;
		}
		// may throw errors
		this->check_file_url();
		this->set_status( 200, "OK" );
		this->load_body( this->req ); ////
		http_header_content_type( this->req, *this );
	} 
	catch (const HTTPError &e) {
		this->set_status( e.getCode(), e.getDescription() );
		this->error_body();
	}
}