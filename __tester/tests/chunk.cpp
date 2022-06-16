#include "chunk_buffer.hpp"
#include "assert_object.hpp"
// valid, head_valid, head, body, remain

void	feed_secure( Asserter &asserter, Chunk_buffer &c, std::string &raw, std::string add )
{
	try
	{
		raw = c.feed_chunk( raw + add );
	}
	catch(const std::exception& e)
	{
		asserter.throw_exception();
	}
}

void	all_tests( Asserter &asserter )
{
	// set a tag for all test of this suite
	asserter.add_tag("chunk1");
	asserter.new_suite("chunk");

	Chunk_buffer c;
	std::string raw;

	feed_secure( asserter, c, raw, "\r\n" );
	asserter.name("empty with \\r\\n");
	asserter.verify_exception();

	asserter.add_tag("chunk2");

	c.clean();
	raw = "";
	feed_secure( asserter, c, raw, "2" );
	asserter.name("empty");
	asserter.verify_assert_equal<Chunk_buffer>( c, Chunk_buffer( false, false, "", "", 0 ) );
	asserter.name("");

	c.clean();
	raw = "";
	feed_secure( asserter, c, raw, "9\r\n" );
	asserter.verify_assert_equal<Chunk_buffer>( c, Chunk_buffer( false, true, "9", "", 9 ) );

	c.clean();
	raw = "";
	feed_secure( asserter, c, raw, "2\r\nab\r\n" );
	asserter.verify_assert_equal<Chunk_buffer>( c, Chunk_buffer( true, true, "2", "ab", 0 ) );

	c.clean();
	raw = "";
	feed_secure( asserter, c, raw, "5\r\nabc" );
	asserter.verify_assert_equal<Chunk_buffer>( c, Chunk_buffer( false, true, "5", "abc", 2 ) );

	c.clean();
	feed_secure( asserter, c, raw, "2\r\n123\r\n" );
	asserter.verify_exception();

	asserter.clear_tag();
	asserter.add_tag("chunk2");

	c.clean();
	raw = "";
	c.set_limits(5, 5);
	feed_secure( asserter, c, raw, "2" );
	feed_secure( asserter, c, raw, "\r\n" );
	feed_secure( asserter, c, raw, "an" );
	feed_secure( asserter, c, raw, "\r\n" );
	asserter.verify_assert_equal<Chunk_buffer>( c, Chunk_buffer( true, true, "2", "an", 0 ) );

	c.clean();
	raw = "";
	feed_secure( asserter, c, raw, "3" );
	feed_secure( asserter, c, raw, "\r" );
	feed_secure( asserter, c, raw, "\n" );
	asserter.verify_assert_equal<Chunk_buffer>( c, Chunk_buffer( false, true, "3", "", 3 ) );

	c.clean();
	raw = "";
	feed_secure( asserter, c, raw, "2" );
	feed_secure( asserter, c, raw, "\r" );
	feed_secure( asserter, c, raw, "\n" );
	feed_secure( asserter, c, raw, "a" );
	feed_secure( asserter, c, raw, "n" );
	feed_secure( asserter, c, raw, "\r" );
	feed_secure( asserter, c, raw, "\n" );
	asserter.verify_assert_equal<Chunk_buffer>( c, Chunk_buffer( true, true, "2", "an", 0 ) );
	asserter.verify_assert_bool( c.is_last() == false );
	asserter.verify_assert_bool( raw == "" );

	c.clean();
	raw = "";
	feed_secure( asserter, c, raw, "2" );
	feed_secure( asserter, c, raw, "\r" );
	feed_secure( asserter, c, raw, "\n" );
	feed_secure( asserter, c, raw, "a" );
	feed_secure( asserter, c, raw, "n" );
	feed_secure( asserter, c, raw, "\r" );
	asserter.verify_assert_equal<Chunk_buffer>( c, Chunk_buffer( false, true, "2", "an", 0 ) );

	c.clean();
	raw = "";
	feed_secure( asserter, c, raw, "2" );
	feed_secure( asserter, c, raw, "\r" );
	feed_secure( asserter, c, raw, "\n" );
	feed_secure( asserter, c, raw, "a" );
	feed_secure( asserter, c, raw, "n" );
	feed_secure( asserter, c, raw, "b" );
	feed_secure( asserter, c, raw, "a" );
	asserter.verify_exception();

	c.clean();
	raw = "";
	feed_secure( asserter, c, raw, "0\r\n\r\n" );
	asserter.verify_assert_bool( c.is_last() == true );

	c.clean();
	raw = "";
	feed_secure( asserter, c, raw, "0" );
	feed_secure( asserter, c, raw, "\r" );
	feed_secure( asserter, c, raw, "\n" );
	feed_secure( asserter, c, raw, "\r" );
	feed_secure( asserter, c, raw, "\n" );
	asserter.verify_assert_bool( c.is_last() == true );
	asserter.verify_assert_bool( raw == "" );

	c.clean();
	raw = "";
	feed_secure( asserter, c, raw, "0" );
	feed_secure( asserter, c, raw, "\r" );
	feed_secure( asserter, c, raw, "\n" );
	feed_secure( asserter, c, raw, "0" );
	feed_secure( asserter, c, raw, "\r" );
	feed_secure( asserter, c, raw, "\n" );
	asserter.verify_exception();

	c.clean();
	raw = "";
	feed_secure( asserter, c, raw, "1q\r\nww\r\n" );
	asserter.verify_exception();
	
	c.clean();
	raw = "";
	feed_secure( asserter, c, raw, "\r\nww\r\n" );
	asserter.verify_exception();

	/* max head size */
	c.clean();
	c.set_limits(5, 5);
	feed_secure( asserter, c, raw, "123456" );
	asserter.verify_exception();

	c.clean();
	c.set_limits(5, 5);
	feed_secure( asserter, c, raw, "7\r\n" );
	asserter.verify_exception();

	c.clean();
	feed_secure( asserter, c, raw, "" );
	asserter.verify_assert_equal<Chunk_buffer>( c, Chunk_buffer( false, false, "", "", 0 ) );

	c.clean();
	raw = "";
	feed_secure( asserter, c, raw, "" );
	feed_secure( asserter, c, raw, "" );
	feed_secure( asserter, c, raw, "" );
	feed_secure( asserter, c, raw, "" );
	feed_secure( asserter, c, raw, "2" );
	feed_secure( asserter, c, raw, "\r" );
	feed_secure( asserter, c, raw, "\nan\r\n" );
	asserter.verify_assert_equal<Chunk_buffer>( c, Chunk_buffer( true, true, "2", "an", 0 ) );
	asserter.verify_assert_bool( raw == "" );

	asserter.clear_tag();
	asserter.end_suite();
}