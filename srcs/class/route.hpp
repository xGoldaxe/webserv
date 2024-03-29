#pragma once

#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include "../utils/string.hpp"
#include "../configuration/redirection.hpp"
#include "../errors/http_code.hpp"

#ifdef DEBUG
# include <iostream>
#endif

#define	DEFAULT_CGI_ENABLE false
#define	DEFAULT_CGI_TIMEOUT -1
#define	DEFAULT_AUTO_INDEX false
#define DEFAULT_SEND_FILE false
#define DEFAULT_FILE_LIMIT 5000
#define DEFAULT_ROUTE_BODY_MAX_SIZE 2048
#define DEFAULT_CHUNK_HEAD_LIMIT_ROUTE 20
#define DEFAULT_CHUNK_BODY_LIMIT_ROUTE 100
#define DEFAULT_MAX_MULTIPART_SIZE_ROUTE 8000
#define DEFAULT_MAX_UPLOAD_SIZE_ROUTE 8000

class Route
{
	private:
		std::string 						root;
		std::string							location;
		std::vector<std::string>			index;
		std::vector<std::string>			methods;
		std::map<int, std::string>			error_pages;
		std::vector<Redirection>			redirections;
		bool								auto_index;
		/* cgi*/
		bool								cgi_enable;
		std::string							cgi_path;
		std::vector<std::string>			cgi_extension;
		int									cgi_timeout;
		bool								send_file;
		int									file_limit;
		int 								body_max_size;
		int									chunk_head_limit;
		int									chunk_body_limit;
		int									max_multipart_size;
		int									max_upload_size;

	public:
		Route( void );
		Route(std::string root);
		Route( std::string location, std::string root );
		Route( std::string location, std::string root, int notdefault);
		Route( std::string location, int notdefault);
		Route( const Route &rhs );
		Route(Route rhs, int notcopy);
		
		~Route(void);

		Route& operator=(const Route& other);

		int 								get_max_multipart_size(void);
		void 								set_max_multipart_size(int max_multipart_size);
		int 								get_max_upload_size(void);
		void 								set_max_upload_size(int max_upload_size);
		int									get_cgi_timeout(void);
		bool								get_send_file(void);
		int									get_file_limit(void);
		bool								get_auto_index( void );
		std::vector<std::string>			get_index(void) const;
		std::string							get_location(void);
		std::string 						get_root(void);
		int 								getBodyMaxSize(void);
		void 								setBodyMaxSize(int body_max_size);
		std::vector<std::string> get_methods(void) const;
		std::vector<Redirection>			&get_redirections(void);
		bool								get_cgi_enable(void);
		std::vector<std::string>			get_cgi_extension(void);
		std::string							get_cgi_path(void);
		std::map<int, std::string>			get_error_pages(void) const;
		void								set_enable_cgi(bool input);
		void								add_cgi_extension(std::string extension);
		void								set_cgi_path(std::string path);
		void								add_error_page( int status_code, std::string error_message );
		void								add_methods( std::string methods );
		void								add_index( std::string index );
		void								add_redirection( int redirect_code, std::string url, std::string redirect_url );
		void								printRoute();
		void								printMethods();
		void								set_auto_index(bool auto_index);
		void 								set_root(std::string root);
		void								set_cgi_timeout(int cgi_timeout);
		void								set_send_file(bool send_file);
		void								set_file_limit(int file_limit);
		bool								is_in_extension(std::string extension);
		Redirection 						return_redirect_url(std::string url) const;
		bool		 						has_redirection(std::string url) const;
		void								check_methods_route(void);
		int									getChunkHeadLimit() const;
		void								setChunkHeadLimit(int chunk_head_limit);
		int									getChunkBodyLimit() const;
		void								setChunkBodyLimit(int chunk_body_limit);
};

Route find_route(std::vector<Route> routes, std::string url, std::string method);
