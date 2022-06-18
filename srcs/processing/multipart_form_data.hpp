#pragma once

#include <iostream>
#include <string>
#include "../errors/http_code.hpp"

#define MAX_MULTIPART_SIZE 8000 // 8mo like PHP

// for state
#define PRE 0
#define IN 1
#define POST 2

class multipart_form_data
{

	private:
		multipart_form_data( void );
		multipart_form_data( multipart_form_data const &src );
		multipart_form_data &   operator=( multipart_form_data const & rhs );

		std::string		buffer;
		std::string		boundary;
		std::size_t		max_size;

		/* actual part */
		std::string		filename;
		std::string		name;
		std::string		body_content;

		int				state;

	public:
		/* coplien */
		multipart_form_data( const std::string &boundary,
								std::size_t max_size = MAX_MULTIPART_SIZE );
		~multipart_form_data( void );
		/* end coplien */

		/* getter */
		std::string	get_buffer(void) const;
		std::string	get_boundary(void) const;
		std::string	get_filename(void) const;
		std::string	get_name(void) const;
		std::string	get_content(void) const;

		/* main functions */
		void	feed( const std::string &str );

		/* verification */
		bool	verify_boundary( const std::string &boundary );
};