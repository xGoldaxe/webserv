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

void	multipart_suite( Asserter &asserter )
{
	std::string boundary;
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
	std::string raw = "salut les amis\r\n--boundary\r\nwe dont care";
	int	type = PRE;
	asserter.verify_assert_equal<std::size_t>( mtpart->find_boundary( mtpart->get_boundary(), raw, type ), 
								static_cast<std::size_t>(14) );
	asserter.add_comment("state");
	asserter.verify_assert_equal<int>( type, IN );

	asserter.name("No \\r\\n before");
	asserter.add_comment("begin");
	raw = "--boundary\r\nwe dont care";
	asserter.verify_assert_equal<std::size_t>( mtpart->find_boundary( mtpart->get_boundary(), raw, type ), 
								static_cast<std::size_t>(0) );

	asserter.add_comment("not begin");
	raw = "some--boundary\r\nwe dont care";
	asserter.verify_assert_equal<std::size_t>( mtpart->find_boundary( mtpart->get_boundary(), raw, type ), 
								std::string::npos );
	
	asserter.name("End");
	raw = "0123456789\r\n--boundary--\r\nwe dont care";
	asserter.verify_assert_equal<std::size_t>( mtpart->find_boundary( mtpart->get_boundary(), raw, type ), 
								static_cast<std::size_t>(10) );
	asserter.add_comment("state");
	asserter.verify_assert_equal<int>( type, POST );

	asserter.delete_last_tag();
	/* <=======================================> */
	asserter.add_tag("parse_part");
	asserter.suite_comment("parse part");

	std::string name;
	std::string filename;
	std::string body;
	raw = "content-disposition: form-data;\
 name=\"form1\"; filename=file.txt\r\nuseless-header: blabla\r\n\
\r\nthis is some serious body";
	parse_part_secure( asserter, mtpart, raw, name, filename, body );
	asserter.verify_assert_equal<std::string>( name, "form1" );
	asserter.verify_assert_equal<std::string>( filename, "file.txt" );
	asserter.verify_assert_equal<std::string>( body, "this is some serious body" );

	asserter.name("no body");
	raw = "content-disposition: form-data;\
 name=\"form1\"; filename=file.txt\r\nuseless-header: blabla\r\n\
\r\n";
	parse_part_secure( asserter, mtpart, raw, name, filename, body );
	asserter.verify_assert_equal<std::string>( name, "form1" );
	asserter.verify_assert_equal<std::string>( filename, "file.txt" );
	asserter.verify_assert_equal<std::string>( body, "" );

	/* <=======================================> */
	asserter.name("invald param");

	raw = "content-disposition: form-data;\
 name=\"form1\"; filename=\r\nuseless-header: blabla\r\n\
\r\nthis is some serious body";
	parse_part_secure( asserter, mtpart, raw, name, filename, body );
	asserter.verify_exception();

	raw = "content-disposition: form-data;\
 name=\"\"\r\nuseless-header: blabla\r\n\
\r\nthis is some serious body";
	parse_part_secure( asserter, mtpart, raw, name, filename, body );
	asserter.verify_exception();

	raw = "content-disposition: form-data;\
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
	raw = "content-disposition: form-data;\
 name=\"form1\r\nuseless-header: blabla";
	parse_part_secure( asserter, mtpart, raw, name, filename, body );
	asserter.verify_exception();
	/* <=======================================> */
// 	asserter.add_tag("feed");
// 	asserter.name("");

// 	boundary = "simple boundary";
// 	mtpart = multipart_construct( asserter, boundary );
// 	asserter.verify_assert_equal<std::string>( mtpart->get_boundary(), "simple boundary" );

// 	asserter.name("normal feed");
// 	raw = "\
// This is the preamble.  It is to be ignored, thoughit\r\n\
// is a handy place for mail composers to include an\r\n\
// explanatory note to non-MIME compliant readers.\r\n\
// --simple boundary\r\n\
// content-disposition: form-data; name=\"field1\"\r\n\
// \r\n\
// This is implicitly typed plain ASCII text.\r\n\
// It does NOT end with a linebreak.\r\n\
// --simple boundary\r\n\
// content-disposition: form-data; name=\"field2\"\r\n\
// \r\n\
// This is implicitly typed plain ASCII text.\r\n\
// It does NOT end with a linebreak.\r\n\
// --simple boundary--\r\n\
// This is the epilogue.  It is also to be ignored.";
// 	mtpart->feed( raw );
	/* <=======================================> */


	asserter.clear_tag();
	asserter.end_suite();
}