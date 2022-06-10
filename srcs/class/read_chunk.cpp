#include "request.hpp"

#define MAX_CHUNK_SIZE 200
/*read chunk*/
/*
WE NEED MAX_CHUNK_SIZE
HEXA TO INT since /r/n ( if other char than hexa throw 400 )
0123456789ABCDEF[\r\n]
x*[char][\r\n]

//didnt throw anything, just set himself to error
*/
bool	try_chunk( std::string chunk_str, int &chunk_size, std::string &content, int &whole_chunk_size )
{
	std::string line = str_read_until( chunk_str, std::string( "\r\n" ) );
	if ( check_and_trunc_line( line ) )
	{
		if ( is_full_hexa( line ) == false ) // do not accept chunk extension!
			throw HTTPCode400();

		chunk_size = hexToInt( line );
		content = str_read_until( chunk_str, std::string( "\r\n" ) );
		if ( check_and_trunc_line( content ) )
		{
			if ( inside.size() != chunk_size )
				throw HTTPCode400();
			return true;
		}
		else if ( inside.size() > chunk_size )
			throw HTTPCode400();
	}
	//else
	if ( line.size() > intToHex(MAX_CHUNK_SIZE).size() + 2 )
		throw HTTPCode400();
	return false;
}


std::size_t	Request::store_chunk( std::string chunk_str )
{
	int chunk_size;
	int	whole_chunk_size;
	std::string content;

	bool is_full = try_chunk( chunk_str, chunk_size, content, whole_chunck_size );
	if ( is_full )
	{
	 	if ( is_full && chunk_size > MAX_CHUNK_SIZE )
		{
	 		this->set_status( 413, "Payload Too Large" );
			return 0;
		}
		if ( chunk_size == 0 )
			this->fulfilled = true;

		*(this->body_file) << content;
	 	return whole_chunk_size;
	}

	return 0;
}