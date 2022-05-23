#ifndef REQUEST_HPP
#	define REQUEST_HPP

#include "../webserv.hpp"

class Request
{

	private:
		webserv_conf	&conf;
		Route			*route;
		std::string		method;
		std::string		url;
		std::string		legacy_url;
		std::string		version;
		std::map<std::string, std::string>		headers;
		std::string		body;
		std::string		row_data;

		Request( void );
	public:
		char			**env;

		/* coplien */
		Request( std::string raw_data, webserv_conf &conf );
		Request( Request const &src );
		~Request( void );

		Request &   operator=( Request const & rhs );
		/* end coplien */
		std::string	getMethod(void) const;
		bool	is_allowed_method( const std::string &method ) const;
		std::string getBody(void);
		std::string getUrl(void);
		std::string getRelativeUrl(void);
		std::string try_url( int *status, std::string *msg );
		Route		*get_route(void);
};


#endif