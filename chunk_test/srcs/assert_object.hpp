#include "chunk.hpp"

bool	try_chunk( std::string chunk_str, size_t &chunk_size, std::string &content, size_t &whole_chunk_size );

test_struct *tracked_struct( bool mode, test_struct *s = NULL )
{
	static test_struct *ss = NULL;
	if ( mode )
		ss = s;
	return ss;
}

bool	track_exception( bool mode, bool is_throw = false )
{
	static bool state = false;
	if ( mode == true )
		state = is_throw;
	return state;
}

void	try_chunk_w( std::string raw )
{
	test_struct *s = tracked_struct( false );
	try
	{
		/* edit this line */
		s->is_chunk = try_chunk( raw, s->chunk_size, s->chunk_content, s->whole_chunk_size );
		track_exception( true, false );
	}
	catch(const std::exception& e)
	{
		track_exception( true, true );
	}
}

#define ADD_TEST 0
#define ADD_SUCCESS 1
#define GET_TEST 2
#define GET_SUCCESS 3

int		result_info( int mode )
{
	static int number_of_test = 0;
	static int success = 0;
	if ( mode == ADD_TEST )
		number_of_test++;
	else if ( mode == ADD_SUCCESS )
		success++;
	else if ( mode == GET_TEST )
		return number_of_test;
	else if ( mode == GET_SUCCESS )
		return success;
	return 0;
}

void	verify_exception()
{
	if ( track_exception(false) == true )
	{
		result_info( ADD_SUCCESS );
		std::cout << "✅ [" << result_info( GET_TEST ) << "] -OK (exception)" << std::endl;
	}
	else
		std::cout << "❌ [" << result_info( GET_TEST ) << "] -KO (no exception)" << std::endl;
	result_info( ADD_TEST );
}

void	verify_assert( test_struct f_s )
{

	test_struct *s = tracked_struct( false );
	bool success = true;
	bool exception = false;
	/* edit this */
	if ( track_exception(false) == true )
	{
		success = false;
		exception = true;
	}
	else
		success = (f_s == *s);
	/* end edit this */

	if ( success )
	{
		result_info( ADD_SUCCESS );
		std::cout << "✅ [" << result_info( GET_TEST ) << "] -OK" << std::endl;
	}
	else
	{
		std::cout << "❌ [" << result_info( GET_TEST ) << "] -KO" <<
		(exception ? " (exception)" : "") << std::endl;
	}
	result_info( ADD_TEST );
	s->clean_up();
}