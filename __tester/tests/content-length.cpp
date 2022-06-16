#include "../internal/assert_object.hpp"

std::size_t	factice( std::size_t size )
{
	return size;
}

void	content_length_test( Asserter &asserter )
{
	// set a tag for all test of this suite
	asserter.new_suite("tests/content-length.cpp");

	std::size_t size;

	asserter.name("");
	size = factice( 10 );
	asserter.verify_assert_equal<std::size_t>( size, 10 );

	size = factice( 10 );
	asserter.verify_assert_equal<std::size_t>( size, 9 );

	asserter.end_suite();

}