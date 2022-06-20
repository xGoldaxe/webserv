#include "multipart_form_data.hpp"

/* ************************************************************************** */
/*                                                                            */
/*            @CONSTRUCTOR                                                    */
/*                                                                            */
/* ************************************************************************** */

multipart_form_data::multipart_form_data( const std::string &boundary, std::size_t max_size )
{
	if ( this->verify_boundary( boundary ) == false )
		throw HTTPCode400();
	this->max_size = max_size;
	this->boundary = boundary;

	this->buffer = "";
	this->filename = "";
	this->name = "";
	this->body_content = "";

	this->state = PRE;
}

multipart_form_data::~multipart_form_data(void)
{}

/* ************************************************************************** */
/*                                                                            */
/*            @GETTERS                                                        */
/*                                                                            */
/* ************************************************************************** */

std::string	multipart_form_data::get_buffer(void) const
{
	return this->buffer;
}
std::string	multipart_form_data::get_boundary(void) const
{
	return this->boundary;
}
std::string	multipart_form_data::get_filename(void) const
{
	return this->filename;
}
std::string	multipart_form_data::get_name(void) const
{
	return this->name;
}
std::string	multipart_form_data::get_content(void) const
{
	return this->body_content;
}
int	multipart_form_data::get_state(void) const
{
	return this->state;
}

/* ************************************************************************** */
/*                                                                            */
/*            @MAIN FUNCTIONS                                                 */
/*                                                                            */
/* ************************************************************************** */

void	multipart_form_data::feed( const std::string &str )
{
	this->buffer += str;
	while (1)
	{
		int type;
		std::size_t find = this->find_boundary( this->boundary, this->buffer, type );
		if ( find == std::string::npos )
		{
			if ( this->buffer.size() > this->max_size )
				throw HTTPCode413();
			break;
		}
		if ( find > this->max_size )
			throw HTTPCode413();
		
		std::string part = this->buffer.substr( 0, find );
		this->buffer = this->buffer.substr( find + this->boundary.size(), this->buffer.size() );
		if ( this->state == IN )
		{
			this->parse_part( part, this->name, this->filename, this->body_content ); // can throw 400
			if ( this->filename != "" )
			{
				this->store_body( this->body_content );
			}
			this->name = "";
			this->filename = "";
			this->body_content = "";
		}
	}
}

#include "../request_parsing/srcs/utils.hpp"

void	multipart_form_data::parse_part( const std::string & part, std::string & name, std::string & filename, std::string & body )
{
	name = "";
	filename = "";
	body = "";
	std::size_t empty_line_index = part.find( "\r\n\r\n" );
	if ( empty_line_index == std::string::npos )
		throw HTTPCode400();
	std::string head = part.substr( 0, empty_line_index );
	std::vector<std::string> lines = preq::read_until( part, &preq::check_line );
	std::map<std::string, std::string> headers = preq::get_headers_req( lines.begin(), lines.end() );

	std::map<std::string, std::string>::iterator cd_head = headers.find( std::string( "content-disposition" ) );
	if ( cd_head == headers.end() )
		throw HTTPCode400();

	std::vector<std::string> params = preq::split_str_c( cd_head->second, ';' );

	for ( std::vector<std::string>::iterator it = params.begin(); it != params.end(); ++it )
		*it = preq::trim( *it, &preq::is_space );

	if ( params.size() < 1 || params[0] != "form-data" )
		throw HTTPCode400();

	if ( params.size() > 1 )
	{
		/** @anchor need to be clean + tested separetly **/
		for ( std::vector<std::string>::iterator it = params.begin() + 1; it != params.end(); ++it )
		{
			std::size_t find = it->find( "=" );
			if ( find == std::string::npos )
				throw HTTPCode400();

			std::string nm = it->substr( 0, find );
			std::string val = it->substr( find + 1, it->size() );
			if ( val.size() < 1 )
				throw HTTPCode400();
			if ( val[0] == '"' )
			{
				if ( val.size() < 3 )
					throw HTTPCode400();
				if ( val[ val.size() - 1 ] != '"' )
					throw HTTPCode400();
				val = val.substr( 1, val.size() - 2 );
			}
			if ( nm == "name" )
				name = val;
			else if ( nm == "filename" )
				filename = val;
		}
	}
	body = part.substr( empty_line_index + 4, part.size() );
}

void	multipart_form_data::store_body( const std::string & body )
{
	(void)body;
}

/* ************************************************************************** */
/*                                                                            */
/*            @VERIFICATIONS                                                  */
/*                                                                            */
/* ************************************************************************** */

bool	multipart_form_data::verify_boundary( const std::string & boundary )
{
	if ( boundary.size() < 1|| boundary.size() > 70 )
		return false;
	if ( boundary.at( boundary.size() - 1 ) == ' ' )
		return false;
	return true;
}

std::size_t	multipart_form_data::find_boundary( const std::string & boundary, const std::string & buffer, int & type )
{
	std::size_t find = buffer.find( "--" + boundary + "\r\n" );
	if ( find == 0 )
	{
		type = IN;
		return 0;
	}
	find = buffer.find( "--" + boundary + "\r\n" );
	if ( find == 0 )
	{
		type = POST;
		return 0;
	}


	find = buffer.find( std::string("\r\n") + "--" + boundary + "\r\n" );
	if ( find == std::string::npos )
	{
		find = buffer.find( "\r\n" + std::string( "--" ) + boundary + std::string( "--" ) + "\r\n");
		if ( find != std::string::npos )
			type = POST;
		return find;
	}
	type = IN;
	return find;
}