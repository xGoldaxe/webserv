#include <iostream>
#include <string>
#include "assert_object.hpp"

void	all_tests( Asserter &asserter );
void	content_length_test( Asserter &asserter );

int	main( int argc, char **argv )
{
	(void)argc;
	Asserter asserter( &argv[1] );
	all_tests( asserter );
	content_length_test( asserter );
}