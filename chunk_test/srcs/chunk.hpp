#ifndef CHUNK_HPP
#	define CHUNK_HPP

#include <iostream>
#include <string>

struct test_struct
{
	size_t chunk_size;
	std::string chunk_content;
	size_t	whole_chunk_size;
	bool is_chunk;

	test_struct()
	{
		is_chunk = false;
	}
	test_struct( bool b, size_t chunk_size = 0,
				std::string chunk_content = "", size_t whole_chunk_size = 0 )
	:
	chunk_size( chunk_size ),
	chunk_content( chunk_content ),
	whole_chunk_size( whole_chunk_size ),
	is_chunk( b )
	{}
	void clean_up(void)
	{
		this->is_chunk = false;
		this->chunk_size = 0;
		this->chunk_content = "";
		this->whole_chunk_size = 0;
	}
	bool	operator==( test_struct & s )
	{
		int success = true;
		if ( this->is_chunk != s.is_chunk )
			success = false;
		if ( this->is_chunk == true )
		{
			if ( this->chunk_size != s.chunk_size )
				this->chunk_size = false;
			if ( this->chunk_content != s.chunk_content )
				success = false;
			if ( this->whole_chunk_size != s.whole_chunk_size )
				success = false;
		}
		return success;
	}
};

void	verify_assert( test_struct f_s );
void	try_chunk_w( std::string raw );
void	verify_exception();
void	all_tests();

#endif