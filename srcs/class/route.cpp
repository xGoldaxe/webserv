#include "route.hpp"
#include "../webserv.hpp"

Route::Route( std::string location, std::string root ) {

	this->root = root;
	this->location = location;
	index.push_back("index.html");
	methods.push_back("GET");
};

Route::Route( void ) {

	this->auto_index = false;
	this->cgi_enable = false;
	index.push_back("index.html");
	methods.push_back("GET");
}

Route::Route( const Route &rhs ) : 
	error_pages(rhs.error_pages), 
	redirections(rhs.redirections), 
	auto_index(rhs.auto_index), 
	cgi_enable(rhs.cgi_enable), 
	cgi_path(rhs.cgi_path), 
	cgi_extension(rhs.cgi_extension)
{
	this->root = finish_by_only_one( rhs.root, '/' );
	this->location = finish_by_only_one( rhs.location, '/' );
	this->index = rhs.index;
	this->methods = rhs.methods;
};

Route::~Route(void) {};

void	Route::enable_cgi( std::string path ) {
	this->cgi_enable = true;
	this->cgi_path = path;
	this->cgi_extension = "php"; //its case sensitive!
}

void	Route::add_error_page( int status_code, std::string error_message ) {

	std::pair<int, std::string> pair( status_code, error_message );
	this->error_pages.insert( pair );
}

void	Route::add_redirection( std::string url, std::string redirect_url ) {

	std::pair<std::string, std::string> pair( url, redirect_url );
	this->redirections.insert( pair );
}

