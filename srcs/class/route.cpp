#include "route.hpp"
#include "../webserv.hpp"

Route::Route( std::string location, std::string root ) {

	this->root = root;
	this->location = location;
	index.push_back("index.html");
	methods.push_back("GET");
};

Route::Route( const Route &rhs ) {

	this->root = finish_by_only_one( rhs.root, '/' );
	this->location = finish_by_only_one( rhs.location, '/' );
	index.push_back("index.html");
	methods.push_back("GET");
};

Route::~Route(void) {};

void	Route::enable_cgi( std::string path ) {
	this->cgi_enable = true;
	this->cgi_path = path;
	this->cgi_extension = "php"; //its case sensitive!
	std::cout << "cgi enabled!" << std::endl;
}