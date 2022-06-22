#include "multipart_form_data.hpp"
#include "../../request_parsing/srcs/req_parse.hpp"
#include "../utils/utils.hpp"

/* ************************************************************************** */
/*                                                                            */
/*            @CONSTRUCTOR                                                    */
/*                                                                            */
/* ************************************************************************** */

multipart_form_data::multipart_form_data( const std::string &boundary, std::size_t max_size, std::size_t max_upload )
{
	this->set_boundary( boundary );
	this->max_size = max_size;
	this->max_upload = max_upload;

	this->buffer = "";
	this->filename = "";
	this->name = "";
	this->body_content = "";

	this->state = PRE;
}

void	multipart_form_data::set_boundary( const std::string &boundary )
{
	if ( this->verify_boundary( boundary ) == false )
		throw HTTPCode400();
	this->boundary = boundary;
}

multipart_form_data::multipart_form_data(void)
{}

multipart_form_data::~multipart_form_data(void)
{}

multipart_form_data &multipart_form_data::operator=( multipart_form_data const &rhs )
{
	this->max_size = rhs.get_max_size();
	this->max_upload = rhs.get_max_upload();
	this->boundary = rhs.get_boundary();

	this->buffer = rhs.get_buffer();
	this->filename = rhs.get_filename();
	this->name = rhs.get_name();
	this->body_content = rhs.get_content();

	this->state = rhs.get_state();
	return *this;
}

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
std::size_t	multipart_form_data::get_max_size(void) const
{
	return this->max_size;
}
std::size_t	multipart_form_data::get_max_upload(void) const
{
	return this->max_upload;
}
std::vector<std::pair<std::string, std::string> >	multipart_form_data::get_files(void) const
{
	return this->files;
}
/* ************************************************************************** */
/*                                                                            */
/*            @MAIN FUNCTIONS                                                 */
/*                                                                            */
/* ************************************************************************** */

void	multipart_form_data::feed( const std::string &str )
{
	this->buffer.append( str.data(), str.size() );
	while (1)
	{
		int type;
		std::size_t boundary_size;
		std::size_t find = this->find_boundary( this->boundary, this->buffer, type, boundary_size );
		if ( find == std::string::npos )
		{
			if ( this->buffer.size() > this->max_size )
				throw HTTPCode413();
			break;
		}
		if ( find > this->max_size )
			throw HTTPCode413();
		
		std::string part = this->buffer.substr( 0, find );
		this->buffer = this->buffer.substr( find + boundary_size, this->buffer.size() );
		if ( this->state == IN )
		{

			this->parse_part( part, this->name, this->filename, this->body_content ); // can throw 400
			if ( this->filename != "" )
			{
				this->files.push_back( std::make_pair<std::string, std::string>( this->filename, this->body_content ) );
			}
			this->name = "";
			this->filename = "";
			this->body_content = "";
		}
		this->state = type;
	}
}

void	multipart_form_data::parse_part( const std::string & part, std::string & name, std::string & filename, std::string & body )
{
	name = "";
	filename = "";
	body = "";
	std::size_t empty_line_index = part.find( "\r\n\r\n" );
	if ( empty_line_index == std::string::npos )
		throw HTTPCode400();

	if ( part.size() - ( empty_line_index + 4 ) > this->max_upload )
		throw HTTPCode413();
	
	std::string head = part.substr( 0, empty_line_index );
	std::vector<std::string> lines = preq::read_until( part, &preq::check_line );
	std::map<std::string, std::string> headers = preq::get_headers_req( lines.begin(), lines.end() );

	std::map<std::string, std::string>::iterator cd_head = headers.find( std::string( "Content-Disposition" ) );
	if ( cd_head == headers.end() )
		throw HTTPCode400();

	std::vector<std::string> params = split_params( cd_head->second );

	if ( params.size() < 1 || params[0] != "form-data" )
		throw HTTPCode400();

	if ( params.size() > 1 )
	{
		std::map<std::string, std::string> p_params = parse_params( params.begin() + 1, params.end() );

		for ( std::map<std::string, std::string>::iterator it = p_params.begin(); it != p_params.end(); ++it )
		{
			if ( it->first == "name" )
				name = it->second;
			else if ( it->first == "filename" )
				filename = it->second;
		}
	}
	body = part.substr( empty_line_index + 4, part.size() );
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

std::size_t	multipart_form_data::find_boundary( const std::string & boundary, const std::string & buffer, int & type, std::size_t & boundary_size )
{
	std::size_t find = buffer.find( "--" + boundary + "\r\n" );
	if ( find == 0 )
	{		
		type = IN;
		boundary_size = 4 + boundary.size();
		return 0;
	}
	find = buffer.find( "--" + boundary + std::string("--") + "\r\n" );
	if ( find == 0 )
	{
		boundary_size = 6 + boundary.size();
		type = POST;
		return 0;
	}

	find = buffer.find( std::string("\r\n") + "--" + boundary + "\r\n" );
	if ( find == std::string::npos )
	{
		find = buffer.find( "\r\n" + std::string( "--" ) + boundary + std::string( "--" ) + "\r\n");
		if ( find != std::string::npos )
		{
			type = POST;
			boundary_size = 8 + boundary.size();
		}
		return find;
	}

	boundary_size = 6 + boundary.size();
	type = IN;
	return find;
}