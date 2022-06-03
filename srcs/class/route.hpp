#pragma once

#include <string>
#include <vector>
#include <map>

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

		Route(void);
		Route( std::string location, std::string root );
		Route( const Route &rhs );
		~Route(void);

		void	enable_cgi( std::string path, std::string extension ) ;
		void	add_error_page( int status_code, std::string error_message );
		void	add_redirection( std::string url, std::string redirect_url );
};
