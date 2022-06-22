#pragma once

#include <algorithm>
#include <string.h>

#include "../utils/string.hpp"
#include "../utils/file.hpp"
#include "../class/route.hpp"
#include "../configuration/webserv.hpp"
#include "../class/chunk_buffer.hpp"
#include "../processing/multipart_form_data.hpp"

#define CHUNKED 2
#define LENGTH 1
#define NO_BODY 0

//state
#define PARSING 0
#define FEEDING 1
#define PROCESSING 2 
#define READY 3
#define INVALID 4

#define CHUNK_HEAD_LIMIT 20
#define CHUNK_BODY_LIMIT 100

class Response;

class Request
{
	private:
		std::string		store_length( std::string add_str );
		std::string		store_chunk( std::string chunck_str );
		std::ofstream	*create_unique_file();
		int 			write_on_file( std::string str );

	protected:
		int 									state;
		Webserv_conf							conf;
		std::string								method;
		std::string								legacy_url;
		std::map<std::string, std::string>		headers;
		std::string								body;
		std::string								query;
		std::string								version;
		bool									request_validity;
		long long int							body_length;
		long long int							remain_body_length;
		std::string								body_file_path;
		int										error_status;
		std::string								error_message;
		std::string								_body_content;
		int										body_transfer;
		bool									fulfilled;
		multipart_form_data						multipart_obj;

		std::ifstream							*processed_file;
		
		/* not copied */
		std::ofstream							*body_file;
		Chunk_buffer							chunk_buffer;

		static std::vector<std::string>	_created_files;
		void							_add_file( std::string filename );
		void							_delete_file( std::string filename );

	public:
		bool			auto_index;
		Route			route;
		char			**env;

		//used as interface to delete all files
		void delete_all_files();

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
		std::string	get_body_file(void) const;
		long long int	get_body_length(void) const;
		int			get_body_transfer(void) const;
		std::string	get_body_content(void) const;

		std::string get_header_value(std::string name) const;

		/* feed body */
		std::string	feed_body( std::string add_str );
		void		try_construct(std::string raw_request, Bundle_server bundle);
		void		check_file_url(void);

		/* processing */
		void		start_processing(void);
		void		process_file(void);


		void							set_status( int status_code, std::string error_message );
		std::pair<int, std::string>		get_status(void) const;


		/* exceptions */
        class invalid_http_request : public std::exception	{
            public:
                virtual const	char* what() const throw()	{
                    return ("Request:Request canno't parse the http raw request.");
                }
        };

		/* headers */
		void	content_length( const std::string &content );
		void	transfer_encoding( const std::string &content );
		void	multipart( const std::string &content );

		/* verification */
		bool	allow_body(void) const;
		int		get_state(void) const;
};

#include "../class/response.hpp"
#include "../http_header/http_header.hpp"
