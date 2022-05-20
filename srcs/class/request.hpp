#ifndef REQUEST_HPP
#	define REQUEST_HPP

#include "../webserv.hpp"
bool is_file(const char* name);
bool file_exist(const std::string& name);

class Request
{

	private:
		webserv_conf	&conf;
		std::string		method;
		std::string		url;
		std::string		version;
		std::map<std::string, std::string>		headers;
		std::string		body;
		std::string		row_data;

		Request( void );
	public:
		/* coplien */
		Request( std::string raw_data, webserv_conf &conf );
		Request( Request const &src );
		~Request( void );

		Request &   operator=( Request const & rhs );
		/* end coplien */
		std::string	getMethod(void);
		std::string getBody(void);
		std::string getUrl(void);
		std::string getRelativeUrl(void);
		std::string tryUrl( int *status );
};


#endif