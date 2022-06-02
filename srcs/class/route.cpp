#include "route.hpp"
#include "../webserv.hpp"

Route::Route(void)
{

	this->auto_index = true;
	this->cgi_enable = false;
	index.push_back("index.html");
	methods.push_back("GET");
}

// default route
Route::Route(std::string location, std::string root) : root(root), location(location)
{
	index.push_back("index.html");
	methods.push_back("GET");
	this->auto_index = true;
	this->cgi_enable = false;
}

Route::Route(Route rhs, int notcopy)
{
	(void)notcopy;
	this->root = finish_by_only_one(rhs.root, '/');
	this->location = finish_by_only_one(rhs.location, '/');
	this->auto_index = true;
	this->cgi_enable = false;
	index.push_back("index.html");
	methods.push_back("GET");
}

Route::Route(std::string location, std::string root, int notdefault) : root(root), location(location)
{
	(void)notdefault;
}

Route::Route(const Route &rhs) : root(rhs.root), location(rhs.location),
								 index(rhs.index), methods(rhs.methods), error_pages(rhs.error_pages), redirections(rhs.redirections),
								 auto_index(rhs.auto_index), cgi_enable(rhs.cgi_enable), cgi_path(rhs.cgi_path), cgi_extension(rhs.cgi_extension)
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
std::map<std::string, std::string> &Route::get_redirections(void)
{
	return this->redirections;
}

bool Route::get_cgi_enable(void)
{
	return this->cgi_enable;
}

std::string Route::get_cgi_extension(void)
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

void Route::enable_cgi(std::string path)
{
	this->cgi_enable = true;
	this->cgi_path = path;
	this->cgi_extension = "php"; // its case sensitive!
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

void Route::add_redirection(std::string url, std::string redirect_url)
{

	std::pair<std::string, std::string> pair(url, redirect_url);
	this->redirections.insert(pair);
}

void Route::printMethods()
{
	std::cout << std::endl;
	unsigned int itp = 0;

	while (itp < this->methods.size())
	{
		std::cout << this->methods[itp];
		itp++;
	}
}

void Route::printRoute()
{
#ifdef DEBUG

	std::vector<std::string>::iterator itp;
	std::vector<std::string>::iterator iti;
	std::map<int, std::string>::iterator ite;

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
	}
	std::cout << std::endl;
#endif
}