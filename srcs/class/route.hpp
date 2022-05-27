#ifndef ROUTE_HPP
#	define ROUTE_HPP

#include <iostream>
#include <string>
#include <vector>
#include <map>

struct Route
{
	std::string 				root;
	std::string					location;
	std::vector<std::string>	index;
	std::vector<std::string>	methods;
	std::map<int, std::string>	error_pages;
	bool						auto_index;
	/* cgi*/
	std::string					cgi_path;
	bool						cgi_enable;
	std::string					cgi_extension;

	Route( std::string location, std::string root );
	Route( const Route &rhs );
	~Route(void);

	void	enable_cgi( std::string path );
	void	add_error_page( int status_code, std::string error_message );
	void	add_methods( std::string methods );
	void	add_index( std::string index );

};


#endif