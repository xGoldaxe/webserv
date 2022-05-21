#ifndef ROUTE_HPP
#	define ROUTE_HPP

#include <iostream>
#include <string>
#include <vector>

struct Route
{
	std::string 				root;
	std::string					location;
	std::vector<std::string>	index;
	std::vector<std::string>	methods;
	/* cgi*/
	std::string					cgi_path;
	bool						cgi_enable;
	std::string					cgi_extension;

	Route( std::string location, std::string root );
	Route( const Route &rhs );
	~Route(void);

	void	enable_cgi( std::string path );
};


#endif