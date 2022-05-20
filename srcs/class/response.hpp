#ifndef RESPONSE_HPP
#	define RESPONSE_HPP

#include "webserv.hpp"

class Response
{

	private:
		webserv_conf	&conf;
		std::string		method;
		std::string		url;
		std::string		version;
		std::map<std::string, std::string>		headers;
		std::string		body;
		std::string		row_data;

		Response( void );
	public:
		/* coplien */
		Response( std::string raw_data, webserv_conf &conf );
		Response( Response const &src );
		~Response( void );

		Response &   operator=( Response const & rhs );
		/* end coplien */
		std::string	getMethod(void);
		std::string getBody(void);
		std::string getUrl(void);
		std::string getRelativeUrl(void);
		std::string tryUrl( int *status );
};


#endif