#include "../webserv.hpp"
#include <algorithm>
#include <string.h>

Request::~Request(void)
{}

Request::Request(void)
{}

/* end coplien */
std::string Request::getMethod(void) const
{
	return (method);
}
std::string Request::getBody(void)
{
	return (body);
}
std::string Request::getUrl(void)
{
	return (url);
}
std::string Request::get_legacy_url(void)
{
	return this->legacy_url;
}
std::string Request::getRelativeUrl(void)
{
	std::cout << "out of date" << std::endl;
	return (this->conf.getServers()[0].getRoot() + url);
}
std::string Request::get_http_version(void) const
{
	return this->version;
}
Route Request::get_route(void)
{
	return (this->route);
}

bool Request::is_request_valid(void) const
{
	return request_validity;
}

bool Request::is_allowed_method(const std::string &method) const
{

	return (std::find(this->route.get_methods().begin(), this->route.get_methods().end(), method) != this->route.get_methods().end());
}

std::string Request::try_url(Response &res)
{

	res.set_status(404, "Not Found"); // fallback if 0 condition has been checked

	try
	{
		std::string redir = this->route.get_redirections().at(
			this->get_legacy_url());

		res.set_status(301, "Moved Permanently");
		res.add_header("Location", redir);
		return (this->url);
	}
	catch (const std::exception &e)
	{
	}

	if (this->route.get_auto_index() && is_file(this->url.c_str()) == 0) // this is a folder
	{
		res.set_status(200, "OK");
		this->auto_index = true;
	}
	else if (file_exist(this->url) && is_file(this->url.c_str()) == 1) // the file exist
	{
		if (file_readable(this->url))
			res.set_status(200, "OK");
		else
			res.set_status(403, "Forbidden");
	}
	else
	{

		std::string test_url = finish_by_only_one(this->url, '/');
		std::vector<std::string> tempPtr = this->conf.getServers()[0].getIndex();
		for (std::vector<std::string>::iterator it = tempPtr.begin();
			 it != tempPtr.end(); ++it)
		{
			test_url += *it;

			if (file_exist(test_url) && is_file(test_url.c_str()) == 1)
			{
				res.set_status(200, "OK");
				this->url = test_url;
			}
		}
	}
	return (this->url);
}

Request::Request( Request const &src )
{
	this->conf = src.conf;
	this->method = src.method;
	this->url = src.url;
	this->legacy_url = src.legacy_url;
	this->headers = src.headers;
	this->body = src.body;
	this->version = src.version;
	this->request_validity = src.request_validity;
	this->auto_index = src.auto_index;
	this->route = src.route;
	this->env = src.env;
}

Request &   Request::operator=( Request const & rhs )
{
	this->conf = rhs.conf;
	this->method = rhs.method;
	this->url = rhs.url;
	this->legacy_url = rhs.legacy_url;
	this->headers = rhs.headers;
	this->body = rhs.body;
	this->version = rhs.version;
	this->request_validity = rhs.request_validity;
	this->auto_index = rhs.auto_index;
	this->route = rhs.route;
	this->env = rhs.env;
	return *this;
}

/* return true if body is fullfiled, or more than fullfiled */
bool	Request::add_body( std::string add_str )
{
	std::map<std::string, std::string> ::iterator it = this->headers.find( "Content-Length" );
	if ( it != this->headers.end() )
	{
		this->body += add_str;
		char	*end_ptr;
		size_t	body_length = strtoul( (it->second).c_str(), &end_ptr, 10 );
		if ( this->body.size() >= body_length )
			return true;

	}
	return false;
}