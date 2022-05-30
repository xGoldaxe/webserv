#include "route.hpp"

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

void	Route::add_redirection( std::string url, std::string redirect_url ) {

	std::pair<std::string, std::string> pair( url, redirect_url );
	this->redirections.insert( pair );
}

void	Route::printRoute()
{
/*

		std::map<int, std::string>			error_pages;
		std::map<std::string, std::string>	redirections;
		bool								auto_index;

		bool								cgi_enable;
		std::string							cgi_path;
		std::string							cgi_extension;
*/
	std::vector<std::string>::iterator itp;
	std::vector<std::string>::iterator iti;
	std::map<int, std::string>::iterator ite;

	std::cout << "***Route***" << std::endl;

	std::cout << "Location : " << this->location << std::endl;

	std::cout << "root : " << this->root << std::endl;

	std::cout << "Methods : ";
	for(itp = this->methods.begin(); itp != this->methods.end(); itp++)
	{
		std::cout << *itp;
	}
	std::cout << std::endl;

	std::cout << "Index : ";
	for(iti = this->index.begin(); iti != this->index.end(); iti++)
	{
		std::cout << *iti;
	}
	std::cout << std::endl;

	std::cout << "Error Pages : ";
	for(ite = this->error_pages.begin(); ite != this->error_pages.end(); ite++)
	{
		std::cout << *iti;
	}
	std::cout << std::endl;	

}