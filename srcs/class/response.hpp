#pragma once

#include <sys/socket.h>
#include <dirent.h>
#include <stdio.h>
#include <vector>

#include "webserv.hpp"

class Response;

#include "request.hpp"

#include "configuration/webserv.hpp"
#include "errors/http_code.hpp"
#include "cgi/cgi_manager.hpp"

#include <sys/types.h>
#include <sys/socket.h>
#include <cstdlib>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define MAX_BODY_LENGTH 5096

#define BODY_TYPE_FILE		1
#define BODY_TYPE_STRING	2

class Response
{
	protected:
		Webserv_conf							conf;
		std::string								version;
		std::map<std::string, std::string>		headers;
		Request									req;
		Request									cpy_req;
		int										_return_body_type;
		std::ifstream							_in_file;
		size_t									_file_len;

		Response &operator=(Response const &rhs);
		Response(void);

	public:
		int				status_code;
		std::string		status_message;
		std::string		body;
		int				client_socket;

		/* typedef */
		typedef std::map<std::string, std::string> headers_t;

		/* coplien */
		Response( int client_socket, Webserv_conf conf, const Request req );
		Response( Response const &src );
		~Response( void );

		/* end coplien */
		std::string	get_str_code(void);
		int	add_header( std::string key, std::string value );
		void set_status( int status_code, std::string msg );
		std::string	load_body( Request &req );
		std::string & error_body(void);
		bool	isFile(void);
		int		send(void);
		int send_chunk(void);
		size_t get_size_next_chunk();
		const Webserv_conf &get_conf() const;
};
