#include "route.hpp"

Route::Route(void) : auto_index(DEFAULT_AUTO_INDEX), cgi_enable(DEFAULT_CGI_ENABLE), cgi_timeout(DEFAULT_CGI_TIMEOUT), send_file(DEFAULT_SEND_FILE), file_limit(DEFAULT_FILE_LIMIT)
{
	index.push_back("index.html");
	methods.push_back("GET");
}

Route::Route(std::string root) : root(root)
{
}

// default route
Route::Route(std::string location, std::string root) : root(root), location(location), auto_index(DEFAULT_AUTO_INDEX), cgi_enable(DEFAULT_CGI_ENABLE), cgi_timeout(DEFAULT_CGI_TIMEOUT), send_file(DEFAULT_SEND_FILE), file_limit(DEFAULT_FILE_LIMIT)
{
	index.push_back("index.html");
	methods.push_back("GET");
}

Route::Route(Route rhs, int notcopy) : error_pages(rhs.error_pages),
									   redirections(rhs.redirections),
									   auto_index(rhs.auto_index),
									   cgi_enable(rhs.cgi_enable),
									   cgi_path(rhs.cgi_path),
									   cgi_extension(rhs.cgi_extension),
									   cgi_timeout(rhs.cgi_timeout),
									   send_file(rhs.send_file),
									   file_limit(rhs.file_limit)
{
	(void)notcopy;
	this->root = finish_by_only_one(rhs.root, '/');
	this->location = finish_by_only_one(rhs.location, '/');
	index.push_back("index.html");
	methods.push_back("GET");
}

Route::Route(std::string location, std::string root, int notdefault) : root(root), location(location), auto_index(DEFAULT_AUTO_INDEX), cgi_enable(DEFAULT_CGI_ENABLE), cgi_timeout(DEFAULT_CGI_TIMEOUT), send_file(DEFAULT_SEND_FILE), file_limit(DEFAULT_FILE_LIMIT)
{
	(void)notdefault;
}

Route::Route(std::string location, int notdefault) : location(location), auto_index(DEFAULT_AUTO_INDEX), cgi_enable(DEFAULT_CGI_ENABLE), cgi_timeout(DEFAULT_CGI_TIMEOUT), send_file(DEFAULT_SEND_FILE), file_limit(DEFAULT_FILE_LIMIT)
{
	(void)notdefault;
}

Route::Route(const Route &rhs) : root(rhs.root), location(rhs.location),
								 index(rhs.index), methods(rhs.methods), error_pages(rhs.error_pages), redirections(rhs.redirections),
								 auto_index(rhs.auto_index), cgi_enable(rhs.cgi_enable), cgi_path(rhs.cgi_path), cgi_extension(rhs.cgi_extension), cgi_timeout(rhs.cgi_timeout),
								 send_file(rhs.send_file), file_limit(rhs.file_limit)
{
}

Route &Route::operator=(const Route &rhs)
{
	if (this != &rhs)
	{
		this->root = rhs.root;
		this->location = rhs.location;
		this->auto_index = rhs.auto_index;
		this->cgi_enable = rhs.cgi_enable;
		this->cgi_extension = rhs.cgi_extension;
		this->cgi_path = rhs.cgi_path;
		this->cgi_timeout = rhs.cgi_timeout;
		this->send_file = rhs.send_file;
		this->file_limit = rhs.file_limit;

		this->index = rhs.index;
		this->methods = rhs.methods;
		this->error_pages = rhs.error_pages;
		this->redirections = rhs.redirections;
	}
	return (*this);
}

Route::~Route(void)
{
}

int Route::get_cgi_timeout(void)
{
	return this->cgi_timeout;
}
bool Route::get_send_file(void)
{
	return this->send_file;
}
int Route::get_file_limit(void)
{
	return this->file_limit;
}

bool Route::get_auto_index(void)
{
	return this->auto_index;
}

std::string Route::get_location(void)
{
	return this->location;
}
std::string Route::get_root(void)
{
	return this->root;
}
std::vector<std::string> Route::get_methods(void) const
{
	return this->methods;
}
std::vector<Redirection> &Route::get_redirections(void)
{
	return this->redirections;
}

bool Route::get_cgi_enable(void)
{
	return this->cgi_enable;
}

std::vector<std::string> Route::get_cgi_extension(void)
{
	return this->cgi_extension;
}

std::string Route::get_cgi_path(void)
{
	return this->cgi_path;
}

std::map<int, std::string> Route::get_error_pages(void) const
{
	return this->error_pages;
}

void Route::set_enable_cgi(bool input)
{
	this->cgi_enable = input;
}
void Route::add_cgi_extension(std::string extension)
{
	this->cgi_extension.push_back(extension);
}
void Route::set_cgi_path(std::string path)
{
	this->cgi_path = path;
}

void Route::add_error_page(int status_code, std::string error_message)
{
	std::pair<int, std::string> pair(status_code, error_message);
	this->error_pages.insert(pair);
}

void Route::add_methods(std::string methods)
{
	this->methods.push_back(methods);
}

void Route::add_index(std::string index)
{
	this->index.push_back(index);
}

void Route::add_redirection(int redirect_code,std::string url, std::string redirect_url)
{
	this->redirections.push_back(Redirection(redirect_code, url, redirect_url));
}

void Route::printMethods()
{
#ifdef DEBUG
	std::cout << std::endl;
	unsigned int itp = 0;

	while (itp < this->methods.size())
	{
		std::cout << this->methods[itp];
		itp++;
	}
#endif
}

void Route::set_root(std::string root)
{
	this->root.clear();
	this->root.append(root);
}

void Route::set_cgi_timeout(int cgi_timeout)
{
	this->cgi_timeout = cgi_timeout;
}
void Route::set_send_file(bool send_file)
{
	this->send_file = send_file;
}
void Route::set_file_limit(int file_limit)
{
	this->file_limit = file_limit;
}

void Route::set_auto_index(bool auto_index)
{
	this->auto_index = auto_index;
}

bool Route::is_in_extension(std::string extension)
{
	std::vector<std::string>::iterator it;
	it = std::find(this->cgi_extension.begin(), this->cgi_extension.end(), extension);
	if (it == this->cgi_extension.end())
	{
		if (*it != extension)
			return false;
	}
	return true;
}

void Route::printRoute()
{
#ifdef DEBUG

	std::vector<std::string>::iterator itp;
	std::vector<std::string>::iterator iti;
	std::map<int, std::string>::iterator ite;
	unsigned int itre = 0;
	std::vector<std::string>::iterator itex;

	std::cout << "***Route***" << std::endl;

	std::cout << "Location : " << this->location << std::endl;

	std::cout << "root : " << this->root << std::endl;

	std::cout << "Methods : ";
	if (!this->methods.empty())
	{
		for (itp = this->methods.begin(); itp != this->methods.end(); itp++)
		{
			std::cout << *itp << " ";
		}
	}
	else
	{
		std::cout << "None";
	}
	std::cout << std::endl;

	std::cout << "Index : ";
	if (!this->index.empty())
	{
		for (iti = this->index.begin(); iti != this->index.end(); iti++)
		{
			std::cout << *iti << " ";
		}
	}
	else
	{
		std::cout << "None";
	}
	std::cout << std::endl;

	std::cout << "Error Pages : " << std::endl;
	if (!this->error_pages.empty())
	{
		for (ite = this->error_pages.begin(); ite != this->error_pages.end(); ite++)
		{
			std::cout << ite->first << " " << ite->second << std::endl;
		}
	}
	else
	{
		std::cout << "None";
		std::cout << std::endl;
	}
	std::cout << "Redirection : " << std::endl;
	if (!this->redirections.empty())
	{
		while(itre < this->redirections.size())
		{
			std::cout << this->redirections[itre].get_redirect_code() << " " << 
			this->redirections[itre].get_url() << " " <<
			this->redirections[itre].get_redirect_url() <<  std::endl;
			itre++;
		}
	}
	else
	{
		std::cout << "None";
		std::cout << std::endl;
	}

	std::cout << "Auto Index : " << this->auto_index << std::endl;

	std::cout << "CGI Enabled : " << this->cgi_enable << std::endl;

	if(!this->cgi_path.empty())
		std::cout << "CGI Path : " << this->cgi_path << std::endl;
	else
		std::cout << "CGI Path : Undefined " << std::endl;

	std::cout << "CGI Extensions : ";
	std::cout << std::endl;
	if (!this->cgi_extension.empty())
	{
		for (itex = this->cgi_extension.begin(); itex != this->cgi_extension.end(); itex++)
		{
			std::cout << *itex << " " << std::endl;
		}
	}
	else
	{
		std::cout << "None";
		std::cout << std::endl;
	}

	std::cout << "CGI Timeout : " << this->cgi_timeout << std::endl;

	std::cout << "Send File : " << this->send_file << std::endl;

	std::cout << "File Limit : " << this->file_limit << std::endl;

	std::cout << std::endl;
#endif
}

Redirection Route::return_redirect_url(std::string url) const
{
	unsigned int i = 0;
	std::string res;

	while(i < this->redirections.size())
	{
		if(this->redirections[i].get_url().compare(url) == 0)
			return this->redirections[i];
		i++;
	}
	throw std::out_of_range("");
}

bool Route::has_redirection(std::string url) const
{
	for (std::vector<Redirection>::const_iterator it = this->redirections.begin(); it != this->redirections.end(); it++)
	{
		std::string redirection_url = finish_by_only_one(it->get_url(), '/');
		if(redirection_url == url)
			return true;
	}
	return false;
}

Route find_route(std::vector<Route> routes, std::string url)
{
	Route route;
	for (std::vector<Route>::iterator it = routes.begin(); it != routes.end(); ++it)
	{
		std::string test_url = finish_by_only_one(url, '/');
		if (test_url == it->get_location() || it->has_redirection(test_url))
			return Route(*it, 1);
	}
	throw HTTPCode404();
}
