#include "test.hpp"

multipart_form_data*	multipart_construct( Asserter &asserter, const std::string &boundary )
{
	try
	{
		multipart_form_data *r = new multipart_form_data( boundary );
		return r;
	}
	catch(const std::exception& e)
	{
		asserter.throw_exception();
	}
	return NULL;
}

void	multipart_suite( Asserter &asserter )
{
	std::string boundary;
	multipart_form_data *mtpart = NULL;
	asserter.new_suite("tests/multipart.cpp");

	asserter.add_tag("verify_boundary");
	asserter.suite_comment("We try the multipart_form_data boundary verification");
	/* <=======================================> */
	asserter.name("Constructor");
	boundary = "cqceqw4w8v4w86V4VRVV";
	mtpart = multipart_construct( asserter, boundary );
	asserter.verify_assert_equal<std::string>( mtpart->get_boundary(), "cqceqw4w8v4w86V4VRVV" );
	/* <=======================================> */
	asserter.name("Valid boundary");
	boundary = "cqceqw4w8v4w86V4VRVV cqceqw4w8v4w86V4VRVV";
	asserter.verify_assert_equal<bool>( mtpart->verify_boundary( boundary ), true );
	/* <=======================================> */
	asserter.name("Empty");
	boundary = "";
	asserter.verify_assert_equal<bool>( mtpart->verify_boundary( boundary ), false );
	/* <=======================================> */
	asserter.name("finish by space");
	boundary = "cqceqw4w8v4w86V4VRVV ";
	asserter.verify_assert_equal<bool>( mtpart->verify_boundary( boundary ), false );
	/* <=======================================> */
	asserter.name("too long");
	boundary = "cqceqw4w8v4w86V4VRVcqceqw4w8v4w86V4VRVcqceqdw4w8v4w86V4VRV0123456789452";
	asserter.verify_assert_equal<bool>( mtpart->verify_boundary( boundary ), false );
	/* <=======================================> */
	delete mtpart;
	asserter.delete_last_tag();

	asserter.add_tag("feed");
	asserter.name("");

	boundary = "simple boundary";
	mtpart = multipart_construct( asserter, boundary );
	asserter.verify_assert_equal<std::string>( mtpart->get_boundary(), "simple boundary" );

	asserter.name("normal feed");
	std::string raw = "\
This is the preamble.  It is to be ignored, thoughit\r\n\
is a handy place for mail composers to include an\r\n\
explanatory note to non-MIME compliant readers.\r\n\
--simple boundary\r\n\
content-disposition: form-data; name=\"field1\"\r\n\
\r\n\
This is implicitly typed plain ASCII text.\r\n\
It does NOT end with a linebreak.\r\n\
--simple boundary\r\n\
content-disposition: form-data; name=\"field2\"\r\n\
\r\n\
This is implicitly typed plain ASCII text.\r\n\
It does NOT end with a linebreak.\r\n\
--simple boundary--\r\n\
This is the epilogue.  It is also to be ignored.";
	mtpart->feed( raw );
	/* <=======================================> */


	asserter.clear_tag();
	asserter.end_suite();
}