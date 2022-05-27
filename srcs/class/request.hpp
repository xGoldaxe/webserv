#ifndef REQUEST_HPP
#	define REQUEST_HPP

#include "webserv_conf.hpp"
#include "response.hpp"
#include "route.hpp"

class Route;
struct Webserv_conf;

class Request
{

	private:
		Webserv_conf	&conf;
		std::string		method;
		std::string		url;
		std::string		legacy_url;
		std::map<std::string, std::string>		headers;
		std::string								body;
		std::string								row_data;
		std::string								version;
		bool									request_validity;

		Request( void );
	public:
		bool			auto_index;
		Route			*route;
		char			**env;

		/* coplien */
		Request( std::string raw_data, Webserv_conf &conf );
		Request( Request const &src );
		~Request( void );

		Request &   operator=( Request const & rhs );
		/* end coplien */
		std::string	getMethod(void) const;
		bool		is_allowed_method( const std::string &method ) const;
		std::string getBody(void);
		std::string getUrl(void);
		std::string get_legacy_url(void);
		std::string getRelativeUrl(void);
		std::string try_url( Response & res );
		Route		*get_route(void);
		bool		is_request_valid(void) const;
		std::string	get_http_version(void) const;

		/* exceptions */
        class invalid_http_request : public std::exception	{
            public:
                virtual const	char* what() const throw()	{
                    return ("Request:Request canno't parse the http raw request.");
                }
        };
};


#endif