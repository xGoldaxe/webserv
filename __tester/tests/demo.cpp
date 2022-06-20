#include "test.hpp"

std::size_t	demo_factice( std::size_t size )
{
	if ( size == 2 )
		throw std::exception();
	return size;
}

std::size_t	try_factice( Asserter &asserter, std::size_t size )
{
	try
	{
		return demo_factice( size );
	}
	catch(const std::exception& e)
	{
		asserter.throw_exception();
	}
	return 0;
}

void	demo_suite( Asserter &asserter )
{
	// set a tag for all test of this suite
	asserter.add_tag("Demo");
	asserter.new_suite("tests/demo.cpp");

	asserter.add_tag("demo_tag");

	asserter.name("demo_test");

	asserter.verify_assert_equal<std::size_t>( demo_factice(0), 0 );
	asserter.verify_assert_equal<std::size_t>( demo_factice(1), 1 );
	asserter.verify_assert_equal<std::size_t>( demo_factice(3), 3 );

	asserter.delete_last_tag();

	asserter.name("demo_exception");
	try_factice( asserter, 2 );
	asserter.verify_exception();

	asserter.name("demo_test_again");
	asserter.verify_assert_equal<std::size_t>( demo_factice(4), 4 );
	asserter.add_comment( "im a precision about this test!");
	asserter.verify_assert_equal<std::size_t>( demo_factice(5), 5 );
	asserter.verify_assert_equal<std::size_t>( demo_factice(6), 6 );

	asserter.add_comment( "we can verify directly from a bool");
	asserter.verify_assert_bool( demo_factice(6) == 6 );

	asserter.clear_tag();
	asserter.end_suite();

	asserter.new_suite("demo/empty");
	asserter.end_suite();

	asserter.new_suite("demo/bad");
	asserter.verify_assert_bool( false );
	asserter.verify_assert_bool( true );
	asserter.verify_assert_bool( false );
	asserter.verify_assert_bool( false );
	asserter.verify_assert_bool( true );
	asserter.end_suite();
}