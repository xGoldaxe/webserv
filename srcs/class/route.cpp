#include "route.hpp"
#include "../webserv.hpp"

Route::Route(std::string location, std::string root)
{

	this->root = root;
	this->location = location;
	index.push_back("index.html");
	methods.push_back("GET");
};

Route::Route(const Route &rhs)
{

	this->root = finish_by_only_one(rhs.root, '/');
	this->location = finish_by_only_one(rhs.location, '/');
	this->auto_index = true;
	this->cgi_enable = false;
	index.push_back("index.html");
	methods.push_back("GET");
};

Route::~Route(void){};

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

void Route::add_index( std::string index )
{
	this->index.push_back(index);
}
