#include "request.hpp"
#include "../../chunk_test/srcs/read_chunk.hpp"

#define MAX_CHUNK_SIZE 200

std::size_t	Request::store_chunk( std::string chunk_str )
{
	size_t			chunk_size;
	size_t			whole_chunk_size;
	std::string 	content;

	try
	{
		if ( chunk_str.size() > MAX_CHUNK_SIZE )
		 	throw HTTPCode413();
		bool is_full = try_chunk( chunk_str, chunk_size, content, whole_chunk_size );
		if ( is_full )
		{
			if ( chunk_size == 0 )
				this->fulfilled = true;
			*(this->body_file) << content;
		 	return whole_chunk_size;
		}
		return 0;
	}
	catch(const HTTPError& e)
	{
		this->set_status( e.getCode(), e.getDescription() );
	}
	return 0;
}