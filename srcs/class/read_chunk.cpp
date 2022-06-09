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
// bool	try_chunk( int &chunk_size, std::string &content, int &whole_chunk_size )

// std::size_t	Request::store_chunk( std::string chunk_str )
// {
// 	int chunk_size;
// 	int	whole_chunk_size;
// 	std::string content;
	

// 	bool is_full = try_chunk( chunk_size, content, whole_chuck_size );
// 	if ( is_full )
// 	{
// 	 	if ( is_full && chunk_size > MAX_CHUNK_SIZE )
// 		{
// 	 		this->set_status( 413, "Payload Too Large" );
// 			return 0;
// 		}
// 		if ( size == 0 )
// 			this->fulfilled = true;

// 		*(this->body_file) << content;
// 	 	return whole_chunk_size;
// 	}

// 	return 0;
// }