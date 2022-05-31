#ifndef ROUTE_HPP
#	define ROUTE_HPP

#include "../webserv.hpp"

class Route
{
	public:
		std::string 						root;
		std::string							location;
		std::vector<std::string>			index;
		std::vector<std::string>			methods;
		std::map<int, std::string>			error_pages;
		std::map<std::string, std::string>	redirections;
		bool								auto_index;
		/* cgi*/
		bool								cgi_enable;
		std::string							cgi_path;
		std::string							cgi_extension;

		Route( std::string location, std::string root );
		Route( std::string location, std::string root, int notdefault);
		Route( const Route &rhs );
		~Route(void);

	void	enable_cgi( std::string path );
	void	add_error_page( int status_code, std::string error_message );
	void	add_methods( std::string methods );
	void	add_index( std::string index );
	void	add_redirection( std::string url, std::string redirect_url );
	void	printRoute();
	void	printMethods();
};


#endif