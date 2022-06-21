#include <iostream>
#include <string>
#include "../class/request.hpp"
#include "../utils/utils.hpp"
#include "../errors/http_code.hpp"

void	Request::content_length( const std::string &content )
{
	if ( this->allow_body() == false )
		throw HTTPCode400();
	if ( is_full( content, "0123456789" ) == false )
		throw HTTPCode400();
	if ( is_more_ll( content ) == true )
		throw HTTPCode400();
	if ( this->body_transfer != NO_BODY )
		throw HTTPCode400();

	this->fulfilled = false;

	char	*end_ptr;
	this->body_length = strtoll( content.c_str(), &end_ptr, 10 );
	this->remain_body_length = body_length;
	this->body_transfer = LENGTH;
	this->fulfilled = ( this->body_length == 0 ? true : false );
}

void	Request::transfer_encoding( const std::string &content )
{
	if ( this->allow_body() == false )
		throw HTTPCode400();

	if ( this->body_transfer != NO_BODY )
		throw HTTPCode400();

	if ( content == std::string("chunked") )
	{
		this->body_length = 0;
		this->body_transfer = CHUNKED;
		this->chunk_buffer.set_limits( CHUNK_HEAD_LIMIT, CHUNK_BODY_LIMIT );
		this->fulfilled = false;
	}
	else
		throw HTTPCode501();
}

void	Request::multipart( const std::string &content )
{
	std::vector<std::string> params = split_params( content );
	if ( params.size() < 1 || params[0] != "multipart/form-data" )
		throw HTTPCode400();
	
	std::map<std::string, std::string> p_params = parse_params( params.begin() + 1, params.end() );
	std::map<std::string, std::string>::iterator b_it =  p_params.find( "boundary" );
	if ( b_it == p_params.end() )
		throw HTTPCode400();
	this->multipart_obj.set_boundary( b_it->second );
}