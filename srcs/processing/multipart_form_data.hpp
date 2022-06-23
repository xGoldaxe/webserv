#pragma once

#include <iostream>
#include <string>
#include <map>
#include <vector>
#include "../errors/http_code.hpp"

/** @todo LINK THIS TO ROUTE CONFIGURATION **/
#define MAX_MULTIPART_SIZE 8000 // 8mo like PHP
#define MAX_UPLOAD_SIZE 8000

// for state
#define PRE 0
#define IN 1
#define POST 2

class multipart_form_data
{

	private:
		multipart_form_data( multipart_form_data const &src );

		std::string		buffer;
		std::string		boundary;
		std::size_t		max_size;
		std::size_t		max_upload;

		/* actual part */
		std::string		filename;
		std::string		name;
		std::string		body_content;

		std::vector<std::pair<std::string, std::string> >	files;

		int				state;

	public:
		/* coplien */
		multipart_form_data( void );
		multipart_form_data( const std::string &boundary,
								std::size_t max_size = MAX_MULTIPART_SIZE, std::size_t max_upload = MAX_UPLOAD_SIZE );
		void	set_boundary( const std::string &boundary );
		~multipart_form_data( void );
		multipart_form_data &   operator=( multipart_form_data const & rhs );
		/* end coplien */

		/* getter */
		std::string	get_buffer(void) const;
		std::string	get_boundary(void) const;
		std::string	get_filename(void) const;
		std::string	get_name(void) const;
		std::string	get_content(void) const;
		int			get_state(void) const;
		std::size_t	get_max_size(void) const;
		std::size_t	get_max_upload(void) const;
		std::vector<std::pair<std::string, std::string> >	get_files(void) const;

		/* main functions */
		void	feed( const std::string &str );
		void	parse_part( const std::string & part, std::string & name, std::string & filename, std::string & body );


		/* verification */
		bool			verify_boundary( const std::string &boundary );
		std::size_t	find_boundary( const std::string & boundary, const std::string & buffer, int & type, std::size_t & boundary_size );
};