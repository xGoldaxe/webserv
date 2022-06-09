#pragma once

#include <sys/socket.h>
#include <dirent.h>
#include <stdio.h>
#include <vector>
#include <cstdlib>

#include "webserv.hpp"

class Response;

#include "request.hpp"

#include "configuration/webserv.hpp"
#include "errors/http_code.hpp"
#include "cgi/cgi_manager.hpp"
#include <sys/socket.h>
#include <sys/un.h>

#define BODY_TYPE_FILE		1
#define BODY_TYPE_STRING	2

class Response
{
	protected:
		Webserv_conf							conf;
		std::string								version;
		std::map<std::string, std::string>		headers;
		Request									*req;
		Request									cpy_req;
		int										_return_body_type;
		std::ifstream							_in_file;
		size_t									_file_len;
		std::string								url;
		const char								*_client_ip;
		size_t									_body_max_size;
		Route									_route;

		Response &operator=(Response const &rhs);
		Response(void);
		Response( Response const &src );

	public:
		int				status_code;
		std::string		status_message;
		std::string		body;
		int				client_socket;

		/* typedef */
		typedef std::map<std::string, std::string> headers_t;

		/* coplien */
		Response(int client_socket, Webserv_conf conf, Request *req, const char *client_ip, size_t max_size, Route route);
		~Response( void );

		/* end coplien */
		std::string	get_str_code(void);
		std::string	get_url(void);
		int	add_header( std::string key, std::string value );
		void set_status( int status_code, std::string msg );
		std::string	load_body();
		std::string & error_body(void);
		bool	isFile(void);
		void output(const size_t req_id);
		int send(void);
		int send_chunk(void);
		size_t get_size_next_chunk();
		size_t getChunkMaxSize();
		const Webserv_conf &get_conf() const;

		/* from req to res */
		void	check_file_url(void);
		bool	is_redirection( std::string &redir_str );
		void	try_url();
};
