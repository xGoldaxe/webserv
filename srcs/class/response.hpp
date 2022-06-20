#pragma once

#include <sys/socket.h>
#include <dirent.h>
#include <stdio.h>
#include <vector>
#include <cstdlib>
#include "../utils/string.hpp"

#include "../webserv.hpp"

class Response;
class CGIManager;

#include "request.hpp"

#include "../configuration/webserv.hpp"
#include "../errors/http_code.hpp"
#include "../cgi/cgi_manager.hpp"
#include <sys/socket.h>
#include <sys/un.h>

#define BODY_TYPE_FILE		1
#define BODY_TYPE_STRING	2
#define BODY_TYPE_CGI		3

class Response
{
	private:
		std::string								version;
		std::map<std::string, std::string>		headers;
		int										_return_body_type;
		std::ifstream							_in_file;
		size_t									_file_len;
		std::string								url;
		const char								*_client_ip;
		size_t									_body_max_size;
		Route									_route;
		std::vector<std::string>				_index;
		bool									_is_custom_error;
		std::string								_body_file;
		CGIManager								*_cgi;

		Response &operator=(Response const &rhs);
		Response( Response const &src );
		Response(void);



	public:
	

		int								status_code;
		std::string						status_message;
		std::string						body;
		int								client_socket;
		Request							*req;

		/* typedef */
		typedef std::map<std::string, std::string> headers_t;

		/* coplien */
		Response(int client_socket, std::vector<std::string> conf, Request *req, const char *client_ip, size_t max_size);
		~Response( void );

		/* end coplien */
		std::string	get_str_code(void);
		std::string	get_url(void);
		int	add_header( std::string key, std::string value );
		void set_status( int status_code, std::string msg );
		std::string	load_body(std::string client_ip);
		std::string & error_body(void);
		bool isFile(void);
		bool isCGI(void);
		size_t getFileSize(void);
		void output(const size_t req_id);
		int send(void);
		int send_chunk(void);
		size_t get_size_next_chunk();
		size_t getChunkMaxSize();

		/* from req to res */
		void	check_file_url(void);
		bool	is_redirection( std::string &redir_str );
		void	try_url(std::string client_ip);
};
