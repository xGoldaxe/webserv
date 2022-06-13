#include "read_chunk.hpp"
#include "assert_object.hpp"

int	main( void )
{
	test_struct *s = new test_struct;
	tracked_struct( true, s );
	all_tests();

	std::cout << result_info( GET_SUCCESS ) << " test(s) pass over " << result_info( GET_TEST ) << " test(s)." << std::endl;

	delete s;
}