#ifndef RESPONSE_HPP
#	define RESPONSE_HPP

#include "../webserv.hpp"

class Response
{

	private:
		const webserv_conf	&conf;
		std::string		version;
		std::map<std::string, std::string>		headers;

		Response( void );
	public:
		int				status_code;
		std::string		status_message;
		std::string		body;
		int				client_socket;

		/* typedef */
		typedef std::map<std::string, std::string> headers_t;

		/* coplien */
		Response( int client_socket, webserv_conf &conf );
		Response( Response const &src );
		~Response( void );

		Response &   operator=( Response const & rhs );
		/* end coplien */
		std::string	get_str_code(void);
		int	add_header( std::string key, std::string value );
		void set_status( int status_code, std::string msg );
		std::string & error_body(void);
		int	send(void);
		const webserv_conf &get_conf() const { return conf; };
};


#endif