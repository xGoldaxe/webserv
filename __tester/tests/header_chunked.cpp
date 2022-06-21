#include "test.hpp"

void	secure_chunked( Asserter &asserter, Request *req, std::string &raw )
{
	try
	{
		return req->transfer_encoding( raw );
	}
	catch(const std::exception& e)
	{
		asserter.throw_exception();
	}
}

void	header_chunked( Asserter &asserter )
{
	std::string raw;
	asserter.new_suite("tests/header_chunked.cpp");

	// /* <=======================================> */
	// asserter.name("Normal input");
	// Request *req = new Request();
	// raw = "chunked";
	// secure_chunked( asserter, req, raw );
	// asserter.verify_assert_equal<long long int>( req->get_body_transfer(), CHUNKED );
	// delete req;
	// /* <=======================================> */
	// asserter.name("Invalid input");
	// req = new Request();
	// raw = "other";
	// secure_chunked( asserter, req, raw );
	// asserter.verify_exception();
	// delete req;
	// /* <=======================================> */
	// asserter.name("Body transfer already defined");
	// req = new Request();
	// raw = "chunked";
	// req->content_length( std::string("10") );
	// asserter.verify_assert_equal<long long int>( req->get_body_length(), 10 );
	// secure_chunked( asserter, req, raw );
	// asserter.verify_exception();
	// delete req;

	asserter.end_suite();
}