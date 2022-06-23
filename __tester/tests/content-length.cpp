#include "test.hpp"

void	secure_length( Asserter &asserter, Request *req, std::string &raw )
{
	try
	{
		return req->content_length( raw );
	}
	catch(const std::exception& e)
	{
		asserter.throw_exception();
	}
}

void	content_length_test( Asserter &asserter )
{
	(void)asserter;
	// std::string raw;
	// asserter.new_suite("tests/content-length.cpp");

	// /* <=======================================> */
	// asserter.name("Normal input");
	// Request *req = new Request();
	// raw = "5488";
	// secure_length( asserter, req, raw );
	// asserter.verify_assert_equal<long long int>( req->get_body_length(), 5488 );
	// asserter.add_comment( "body transfer" );
	// asserter.verify_assert_equal<long long int>( req->get_body_transfer(), LENGTH );
	// delete req;
	// /* <=======================================> */
	// asserter.name("With letters");
	// req = new Request();
	// raw = "abc";
	// secure_length( asserter, req, raw );
	// asserter.verify_exception();
	// delete req;
	// /* <=======================================> */
	// asserter.name("With minus");
	// req = new Request();
	// raw = "-50";
	// secure_length( asserter, req, raw );
	// asserter.verify_exception();
	// delete req;
	// /* <=======================================> */
	// asserter.name("Long long max");
	// req = new Request();
	// raw = "9223372036854775807";
	// secure_length( asserter, req, raw );
	// asserter.verify_assert_equal<long long int>( req->get_body_length(), 9223372036854775807 );
	// delete req;
	// /* <=======================================> */
	// asserter.name("Long long max + 1");
	// req = new Request();
	// raw = "9223372036854775484995955808";
	// secure_length( asserter, req, raw );
	// asserter.verify_exception();
	// delete req;
	// /* <=======================================> */
	// asserter.name("Body transfer already defined");
	// req = new Request();
	// raw = "chunked";
	// secure_chunked( asserter, req, raw );
	// asserter.verify_assert_equal<long long int>( req->get_body_transfer(), CHUNKED );
	// secure_length( asserter, req, raw );
	// asserter.verify_exception();
	// delete req;
	// /* <=======================================> */

	// asserter.end_suite();
}