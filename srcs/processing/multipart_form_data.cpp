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
			break;
		std::string part = this->buffer.substr( 0, find );
		this->buffer = this->buffer.substr( find + this->boundary.size(), this->buffer.size() );
		if ( this->state == IN )
		{
			this->parse_part( this->name, this->filename, this->body_content ); // can throw 400
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

void	multipart_form_data::parse_part( std::string & name, std::string & filename, std::string & body )
{
	(void)name;
	(void)filename;
	(void)body;
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
	std::size_t find = buffer.find( std::string("\r\n") + "--" + boundary + "\r\n" );
	if ( find == std::string::npos )
	{
		find = buffer.find( "\r\n" + std::string( "--" ) + boundary + std::string( "--" ) + "\r\n");
		if ( find != std::string::npos )
			type = POST;
	}
	else
		type = IN;
	return find;
}