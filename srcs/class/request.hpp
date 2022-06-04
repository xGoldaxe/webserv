#pragma once

#include <algorithm>
#include <string.h>

#include "utils/go_through_until.hpp"
#include "utils/string.hpp"
#include "utils/file.hpp"

#include "route.hpp"
#include "response.hpp"
#include "configuration/webserv.hpp"
#include "http_header/http_header.hpp"

class Response;

class Request
{

	private:
		Webserv_conf							&conf;
		std::string								method;
		std::string								url;
		std::string								legacy_url;
		std::map<std::string, std::string>		headers;
		std::string								body;
		std::string								version;
		bool									request_validity;

		Request( void );
	public:
		bool			auto_index;
		Route			route;
		char			**env;

		/* coplien */
		Request( int socket_data, Webserv_conf &conf );
		Request( Request const &src );
		~Request( void );

		Request &   operator=( Request const & rhs );
		/* end coplien */
		/* fill from parsed req */
		void		fill_start_line( std::string method,
											std::string url,
											std::string version );
		void		fill_headers( std::map<std::string, std::string> headers );
		void		fill_body( std::string body );
		/* fill from parsed req */


		std::string	getMethod(void) const;
		bool		is_allowed_method( const std::string &method ) const;
		std::string getBody(void);
		std::string getUrl(void);
		std::string get_legacy_url(void);
		std::string getRelativeUrl(void);
		Route		get_route(void);
		bool		is_request_valid(void) const;
		std::string	get_http_version(void) const;

		void		try_url( Response & res );
		void		check_file_url(void);
		bool		is_redirection( std::string &redir_str );


		/* exceptions */
        class invalid_http_request : public std::exception	{
            public:
                virtual const	char* what() const throw()	{
                    return ("Request:Request canno't parse the http raw request.");
                }
        };
};