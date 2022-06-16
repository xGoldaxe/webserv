#include <iostream>
#include <string>
#include "internal/assert_object.hpp"

void	demo_suite( Asserter &asserter );
void	all_tests( Asserter &asserter );
void	content_length_test( Asserter &asserter );
void	util_test( Asserter &asserter );

int	main( int argc, char **argv )
{
	(void)argc;
	Asserter asserter( &argv[1] );
	demo_suite( asserter );
	all_tests( asserter );
	content_length_test( asserter );
	util_test( asserter );
}