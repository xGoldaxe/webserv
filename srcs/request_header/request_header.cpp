#include <iostream>
#include <string>
#include "../class/request.hpp"
#include "../utils/utils.hpp"
#include "../errors/http_code.hpp"

void	Request::content_length( const std::string &content )
{
	if ( is_full( content, "0123456789" ) == false )
		throw HTTPCode400();
	if ( is_more_ll( content ) == true )
		throw HTTPCode400();
	if ( this->body_transfer != NO_BODY )
		throw HTTPCode400();

	char	*end_ptr;
	this->body_length = strtoll( content.c_str(), &end_ptr, 10 );
	this->remain_body_length = body_length;
	this->body_transfer = LENGTH;
	if ( this->body_length == 0 )
		this->fulfilled = true;
}

void	Request::transfer_encoding( const std::string &content )
{
	if ( this->body_transfer != NO_BODY )
		throw HTTPCode400();

	if ( content == std::string("chunked") )
	{
		this->body_length = 0;
		this->body_transfer = CHUNKED;
		this->chunk_buffer.set_limits( CHUNK_HEAD_LIMIT, CHUNK_BODY_LIMIT );
	}
	else
		throw HTTPCode501();
}