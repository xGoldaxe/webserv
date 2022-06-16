#include "../internal/assert_object.hpp"
#include "../../srcs/utils/utils.hpp"

void	util_test( Asserter &asserter )
{
	// set a tag for all test of this suite
	asserter.add_tag("Utils");
	asserter.new_suite("tests/utils.cpp");

	asserter.add_tag("is_full_hexa");

	asserter.name("full_hexa");
	asserter.verify_assert_equal<bool>( is_full_hexa( "aaa" ), true );
	asserter.verify_assert_equal<bool>( is_full_hexa( "sss" ), false );
	asserter.verify_assert_equal<bool>( is_full_hexa( "508674" ), true );
	asserter.verify_assert_equal<bool>( is_full_hexa( "" ), true );

	asserter.delete_last_tag();
	asserter.verify_assert_equal<bool>( is_full_hexa( "" ), true );

	asserter.clear_tag();
	asserter.end_suite();
}