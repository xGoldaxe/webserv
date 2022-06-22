#include "test.hpp"
#include "../../srcs/utils/utils.hpp"

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

void	parse_part_secure( Asserter &asserter, multipart_form_data *mtpart,
std::string raw, std::string & name, std::string & filename, std::string & body )
{
	try
	{
		mtpart->parse_part( raw, name, filename, body );
	}
	catch(const std::exception& e)
	{
		asserter.throw_exception();
	}
}

void	try_header_multipart( Asserter &asserter, Request *req, std::string raw )
{
	try
	{
		req->multipart( raw );
	}
	catch(const std::exception& e)
	{
		asserter.throw_exception();
	}
}

void	secure_feed( Asserter &asserter, multipart_form_data *mtpart, std::string raw )
{
	try
	{
		mtpart->feed( raw );
	}
	catch(const std::exception& e)
	{
		asserter.throw_exception();
	}
}


void	multipart_suite( Asserter &asserter )
{
	std::string boundary;
	std::string raw;
	multipart_form_data *mtpart = NULL;
	asserter.add_tag("multipart");
	asserter.new_suite("tests/multipart.cpp");
	asserter.delete_last_tag();

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
	/* <=======================================> */
	asserter.name("find_boundary");
	asserter.add_tag("find_boundary");
	asserter.suite_comment("find boundary");

	boundary = "boundary";
	mtpart = multipart_construct( asserter, boundary );
	asserter.verify_assert_equal<std::string>( mtpart->get_boundary(), "boundary" );

	asserter.name("Normal find");
	raw = "salut les amis\r\n--boundary\r\nwe dont care";
	int	type = PRE;
	std::size_t readed = 0;
	asserter.verify_assert_equal<std::size_t>( mtpart->find_boundary( mtpart->get_boundary(), raw, type, readed ), 
								static_cast<std::size_t>(14) );
	asserter.add_comment("state");
	asserter.verify_assert_equal<int>( type, IN );

	asserter.name("No \\r\\n before");
	asserter.add_comment("begin");
	raw = "--boundary\r\nwe dont care";
	asserter.verify_assert_equal<std::size_t>( mtpart->find_boundary( mtpart->get_boundary(), raw, type, readed ), 
								static_cast<std::size_t>(0) );

	asserter.add_comment("not begin");
	raw = "some--boundary\r\nwe dont care";
	asserter.verify_assert_equal<std::size_t>( mtpart->find_boundary( mtpart->get_boundary(), raw, type, readed ), 
								std::string::npos );
	
	asserter.name("End");
	raw = "0123456789\r\n--boundary--\r\nwe dont care";
	asserter.verify_assert_equal<std::size_t>( mtpart->find_boundary( mtpart->get_boundary(), raw, type, readed ), 
								static_cast<std::size_t>(10) );
	asserter.add_comment("state");
	asserter.verify_assert_equal<int>( type, POST );

	asserter.delete_last_tag();
	/* <=======================================> */
	asserter.add_tag("parse_part");
	asserter.suite_comment("parse part");

	asserter.name("Normal");
	std::string name;
	std::string filename;
	std::string body;
	raw = "Content-Disposition: form-data;\
 name=\"form1\"; filename=file.txt\r\nuseless-header: blabla\r\n\
\r\nthis is some serious body";
	parse_part_secure( asserter, mtpart, raw, name, filename, body );
	asserter.verify_assert_equal<std::string>( name, "form1" );
	asserter.verify_assert_equal<std::string>( filename, "file.txt" );
	asserter.verify_assert_equal<std::string>( body, "this is some serious body" );

	asserter.name("no body");
	raw = "Content-Disposition: form-data;\
 name=\"form1\"; filename=file.txt\r\nuseless-header: blabla\r\n\
\r\n";
	parse_part_secure( asserter, mtpart, raw, name, filename, body );
	asserter.verify_assert_equal<std::string>( name, "form1" );
	asserter.verify_assert_equal<std::string>( filename, "file.txt" );
	asserter.verify_assert_equal<std::string>( body, "" );

	/* <=======================================> */
	asserter.name("invald param");

	raw = "Content-Disposition: form-data;\
 name=\"form1\"; filename=\"\r\nnuseless-header: blabla\r\n\
\r\nthis is some serious body";
	parse_part_secure( asserter, mtpart, raw, name, filename, body );
	asserter.verify_exception();


	raw = "Content-Disposition: form-data;\
 name=\"form1\r\nuseless-header: blabla\r\n\
\r\nthis is some serious body";
	parse_part_secure( asserter, mtpart, raw, name, filename, body );
	asserter.verify_exception();

	asserter.name("no content disposition");
	raw = "useless-header: blabla\r\n\
\r\nthis is some serious body";
	parse_part_secure( asserter, mtpart, raw, name, filename, body );
	asserter.verify_exception();

	asserter.name("no empty line");
	raw = "Content-Disposition: form-data;\
 name=\"form1\r\nuseless-header: blabla";
	parse_part_secure( asserter, mtpart, raw, name, filename, body );
	asserter.verify_exception();
	/* <=======================================> */
	asserter.suite_comment( "request header" );
	asserter.add_tag("header_multipart");
	asserter.name("Empty boundary");
	Request *req = new Request();
	raw = " multipart/form-data; boundary=\"\"";
	try_header_multipart( asserter, req, raw );
	asserter.verify_exception();

	asserter.name("Not multipart/form-data");
	raw = " multipart/form-dqdqw; boundary=\"boundary\"";
	try_header_multipart( asserter, req, raw );
	asserter.verify_exception();

	asserter.name("No boundary");
	raw = " multipart/form-dqdqw; other=\"boundary\"";
	try_header_multipart( asserter, req, raw );
	asserter.verify_exception();

	asserter.name("Nothing");
	raw = "";
	try_header_multipart( asserter, req, raw );
	asserter.verify_exception();

	asserter.name("Good");
	raw = " multipart/form-data; boundary=\"boundary\"";
	try_header_multipart( asserter, req, raw );
	asserter.add_comment( "no exception" );
	asserter.verify_assert_bool( true );
	delete req;
	asserter.delete_last_tag();

	/* <=======================================> */
	asserter.add_tag("feed");
	asserter.name("");
	asserter.suite_comment("feed");

	boundary = "simple boundary";
	mtpart = multipart_construct( asserter, boundary );
	asserter.verify_assert_equal<std::string>( mtpart->get_boundary(), "simple boundary" );

	asserter.name("normal feed");
	raw = "\
This is the preamble.  It is to be ignored, thoughit\r\n\
is a handy place for mail composers to include an\r\n\
explanatory note to non-MIME compliant readers.\r\n\
--simple boundary\r\n\
Content-Disposition: form-data; name=\"field1\"\r\n\
\r\n\
This is implicitly typed plain ASCII text.\r\n\
It does NOT end with a linebreak.\r\n\
--simple boundary\r\n\
Content-Disposition: form-data; name=\"field2\"\r\n\
\r\n\
This is implicitly typed plain ASCII text.\r\n\
It does NOT end with a linebreak.\r\n\
--simple boundary\r\n\
Content-Disposition: form-data; name=\"field2\"; filename=\"test.txt\"\r\n\
\r\n\
this content will be store\r\n\
--simple boundary--\r\n\
This is the epilogue.  It is also to be ignored.";
	mtpart->feed( raw );
	asserter.verify_assert_bool( true );

	delete mtpart;
	mtpart = multipart_construct( asserter, boundary );
	asserter.name("normal feed");
	raw = "--simple boundary\r\n\
Content-Disposition: form-data; name=\"field1\"\r\n\
\r\n\
This is implicitly typed plain ASCII text.\r\n\
It does NOT end with a linebreak.\r\n\
--simple boundary\r\n\
Content-Disposition: form-data; name=\"field2\"\r\n\
\r\n\
This is implicitly typed plain ASCII text.\r\n\
It does NOT end with a linebreak.\r\n\
--simple boundary\r\n\
Content-Disposition: form-data; name=\"field2\"; filename=\"test.txt\"\r\n\
\r\n\
this content will be store\r\n\
--simple boundary\r\n\
";
	mtpart->feed( raw );
	asserter.verify_assert_bool( true );
	
	delete mtpart;
	mtpart = multipart_construct( asserter, boundary );
	asserter.name("filename with no body");
	raw = "--simple boundary\r\n\
Content-Disposition: form-data; name=\"field1\"; filename=\"test.txt\"\r\n\
\r\n\r\n\
--simple boundary--\r\n";
	secure_feed( asserter, mtpart, raw );
	asserter.verify_assert_bool( true );
	
	delete mtpart;
	mtpart = multipart_construct( asserter, boundary );
	raw = "\
Content-Disposition: form-data; name=\"field1\"\r\n\
\r\n\
This is implicitly typed plain ASCII text.\r\n\
It does NOT end with a linebreak.\r\n\
";
	mtpart->feed( raw );
	asserter.verify_assert_bool( true );

	delete mtpart;
	mtpart = multipart_construct( asserter, boundary );
	asserter.name("normal feed");
	raw = "--simple boundary\r\n\
Content-Disposition: form-datadqd; name=\"field1\"\r\n\
\r\n\
This is implicitly typed plain ASCII text.\r\n\
It does NOT end with a linebreak.\r\n\
--simple boundary\r\n\
Content-Disposition: form-data; name=\"field2\"\r\n\
\r\n\
This is implicitly typed plain ASCII text.\r\n\
It does NOT end with a linebreak.\r\n\
--simple boundary\r\n\
Content-Disposition: form-data; name=\"field2\"; filename=\"test.txt\"\r\n\
\r\n\
this content will be store\r\n\
--simple boundary\r\n\
";
	secure_feed( asserter, mtpart, raw );
	asserter.verify_exception();

	delete mtpart;
	mtpart = multipart_construct( asserter, boundary );
	raw = "--simple boundary\r\n\
Content-Disposition: form-data; name=\"field1\"\r\n\
\r\n\
This is implicitly typed plain ASCII text.\r\n\
It does NOT end with a linebreak.\r\n\
--simple boundary\r\n\
Content-Disposition: form-data; name=\"field2\"\r\n\
\r\n\
This is implicitly typed plain ASCII text.\r\n\
It does NOT end with a linebreak.\r\n\
--simple boundary\r\n\
\r\n\
this content will be store\r\n\
--simple boundary\r\n\
";
	secure_feed( asserter, mtpart, raw );
	asserter.verify_exception();
	delete mtpart;
	/* <=======================================> */

	asserter.clear_tag();
	asserter.end_suite();
}