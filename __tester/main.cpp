#include <iostream>
#include <string>
#include "tests/test.hpp"

int	main( int argc, char **argv )
{
	(void)argc;
	Asserter asserter( &argv[1] );
	demo_suite( asserter );
	all_tests( asserter );
	content_length_test( asserter );
	util_test( asserter );
	header_chunked( asserter );
	multipart_suite( asserter );
}