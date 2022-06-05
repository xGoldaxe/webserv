#pragma once

#include "request.hpp"
#include "webserv_conf.hpp"

class Response
{

	protected:
		Webserv_conf							conf;
		std::string								version;
		std::map<std::string, std::string>		headers;
		const Request							*req;

	public:
		int				status_code;
		std::string		status_message;
		std::string		body;
		int				client_socket;

		/* typedef */
		typedef std::map<std::string, std::string> headers_t;

		/* coplien */
		Response( void );
		Response( int client_socket, Webserv_conf conf, const Request *req );
		Response( Response const &src );
		Response &   operator=( Response const & rhs );
		~Response( void );

		/* end coplien */
		std::string	get_str_code(void);
		int	add_header( std::string key, std::string value );
		void set_status( int status_code, std::string msg );
		std::string	load_body( Request &req );
		std::string & error_body(void);
		int	send(void);
		const Webserv_conf &get_conf() const;
};
