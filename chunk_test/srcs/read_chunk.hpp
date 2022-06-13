#ifndef READ_CHUNK_HPP
#	define READ_CHUNK_HPP

#include <iostream>
#include <string>
#include <vector>
#include "../../srcs/errors/http_code.hpp"
#include "utils.hpp"

// this is not exactly how it should behave, need to modify its fast!
bool	try_chunk( std::string chunk_str, size_t &chunk_size, std::string &content, size_t &whole_chunk_size )
{
	whole_chunk_size = chunk_str.size();
	std::vector<std::string> parts = read_until( chunk_str, &check_line, 2 );
	if ( parts.size() != 2 )
		return false;
	if ( check_and_trunc_line( &parts[0] ) )
	{
	 	if ( is_full_hexa( parts[0] ) == false ) // do not accept chunk extension!
			throw HTTPCode400();
		chunk_size = hexToInt( parts[0] );
		content = parts[1];
		if ( check_and_trunc_line( &content ) )
		{
		 	if ( content.size() != chunk_size )
		 		throw HTTPCode400();
		 	return true;
		}
	}
	else
		return false;
	return false;
}

#endif