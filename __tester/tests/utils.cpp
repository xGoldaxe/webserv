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

	asserter.name("is full");
	asserter.add_tag("bool");
	asserter.add_tag("int");

	asserter.verify_assert_equal<bool>( is_full( "123", "0123456789" ), true );
	asserter.verify_assert_equal<bool>( is_full( "", "0123456789" ), true );
	asserter.verify_assert_equal<bool>( is_full( "a123", "0123456789" ), false );
	asserter.verify_assert_equal<bool>( is_full( "123a", "0123456789" ), false );
	asserter.verify_assert_equal<bool>( is_full( "1a23", "0123456789" ), false );

	asserter.delete_last_tag();
	asserter.delete_last_tag();
	
	asserter.suite_comment("is more ll");
	asserter.add_tag("is_more_ll");
	asserter.add_tag("long long int");

	asserter.name("normal");
	std::string s = "864";
	asserter.verify_assert_equal<bool>( is_more_ll( s ), false );
	asserter.name("max long long int");
	s = "9223372036854775807";
	asserter.verify_assert_equal<bool>( is_more_ll( s ), false );
	asserter.name("max long long int + 1");
	s = "9223372036854775808";
	asserter.verify_assert_equal<bool>( is_more_ll( s ), true );
	asserter.name("20 char");
	s = "11111111111111111111";
	asserter.verify_assert_equal<bool>( is_more_ll( s ), true );

	asserter.clear_tag();
	asserter.end_suite();
}