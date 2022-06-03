#pragma once

#include <string>
#include <vector>
#include <map>

class Route
{
	private:
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

	public:
		Route( void );
		Route(std::string root);
		Route( std::string location, std::string root );
		Route( std::string location, std::string root, int notdefault);
		Route( const Route &rhs );
		Route(Route rhs, int notcopy);
		
		~Route(void);

		Route& operator=(const Route& other);

	bool								get_auto_index( void );
	std::string							get_location(void);
	std::string 						get_root(void);
	std::vector<std::string> 			get_methods(void) const;
	std::map<std::string, std::string>  &get_redirections(void);
	bool								get_cgi_enable(void);
	std::string							get_cgi_extension(void);
	std::string							get_cgi_path(void);
	std::map<int, std::string>			get_error_pages(void) const;
	void								enable_cgi(std::string path, std::string extension);
	void								add_error_page( int status_code, std::string error_message );
	void								add_methods( std::string methods );
	void								add_index( std::string index );
	void								add_redirection( std::string url, std::string redirect_url );
	void								printRoute();
	void								printMethods();
	void 								set_root(std::string root);

};

