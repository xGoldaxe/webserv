#pragma once

#include <algorithm>
#include <string.h>

#include "utils/string.hpp"
#include "utils/file.hpp"
#include "route.hpp"
#include "configuration/webserv.hpp"

#define CHUNKED 2
#define LENGTH 1
#define NO_BODY 0

class Response;

class Request
{
	private:
		std::size_t	store_length( std::string add_str );
		std::size_t	store_chunk( std::string chunck_str );
		std::ofstream	*create_unique_file( std::string path );

	protected:
		Webserv_conf							conf;
		std::string								method;
		std::string								legacy_url;
		std::map<std::string, std::string>		headers;
		std::string								body;
		std::string								query;
		std::string								version;
		bool									request_validity;
		std::size_t								body_length;
		std::ofstream							*body_file;
		std::string								body_file_path;
		int										error_status;
		std::string								error_message;
		int										body_transfer;
		bool									fulfilled;

	public:
		bool			auto_index;
		Route			route;
		char			**env;

		/* coplien */
		Request( void );
		~Request( void );

		Request &   operator=( Request const & rhs );
		Request( Request const &src );
		/* end coplien */
		/* fill from parsed req */
		void		fill_start_line( std::string method,
											std::string url,
											std::string version );
		void		fill_headers( std::map<std::string, std::string> headers );
		void		fill_body( std::string body );
		void		fill_query(std::string query);
		/* fill from parsed req */

		std::string	getMethod(void) const;
		bool		is_allowed_method( const std::string &method ) const;
		std::string getBody(void);
		std::string get_query(void) const;
		std::string get_legacy_url(void) const;
		Route		get_route(void);
		bool		is_request_valid(void) const;
		std::string	get_http_version(void) const;

		std::string get_header_value(std::string name) const;

		std::size_t	feed_body( std::string add_str );
		bool		is_fulfilled(void) const;
		void		try_construct(std::string raw_request, std::vector<Route> routes);
		void		check_file_url(void);

		void							set_status( int status_code, std::string error_message );
		std::pair<int, std::string>		get_status(void) const;


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
