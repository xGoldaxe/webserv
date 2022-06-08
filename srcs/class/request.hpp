#pragma once

#include <algorithm>
#include <string.h>

#include "utils/string.hpp"
#include "utils/file.hpp"
#include "route.hpp"
#include "configuration/webserv.hpp"

class Response;

class Request
{

	protected:
		Webserv_conf							conf;
		std::string								method;
		std::string								url;
		std::string								legacy_url;
		std::map<std::string, std::string>		headers;
		std::string								body;
		std::string								version;
		bool									request_validity;
		std::size_t								body_length;

	public:
		bool			auto_index;
		Route			route;
		char			**env;

		/* coplien */
		Request( void );
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
		std::string get_legacy_url(void) const;
		std::string getRelativeUrl(void);
		Route		get_route(void);
		bool		is_request_valid(void) const;
		std::string	get_http_version(void) const;

		std::string get_header_value(std::string name) const;

		std::size_t	feed_body( std::string add_str );
		bool		is_fulfilled(void) const;
		void		try_construct( std::string raw_request, Webserv_conf conf );
		void		try_url( Response * res );
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

#include "response.hpp"
#include "http_header/http_header.hpp"
