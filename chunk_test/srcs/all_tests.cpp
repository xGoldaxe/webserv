#include "../../srcs/class/chunk_buffer.hpp"
#include "assert_object.hpp"
// valid, head_valid, head, body, remain

void	feed_secure( Chunk_buffer &c, std::string &raw, std::string add )
{
	try
	{
		raw = c.feed_chunk( raw + add );
	}
	catch(const std::exception& e)
	{
		track_exception( true, true );
	}
}

void	all_tests()
{
	Chunk_buffer c;
	std::string raw;

	feed_secure( c, raw, "\r\n" );
	verify_exception();

	c.clean();
	raw = "";
	feed_secure( c, raw, "2" );
	verify_assert_equal<Chunk_buffer>( c, Chunk_buffer( false, false, "", "", 0 ) );

	c.clean();
	raw = "";
	feed_secure( c, raw, "9\r\n" );
	verify_assert_equal<Chunk_buffer>( c, Chunk_buffer( false, true, "9", "", 9 ) );

	c.clean();
	raw = "";
	feed_secure( c, raw, "2\r\nab\r\n" );
	verify_assert_equal<Chunk_buffer>( c, Chunk_buffer( true, true, "2", "ab", 0 ) );

	c.clean();
	raw = "";
	feed_secure( c, raw, "5\r\nabc" );
	verify_assert_equal<Chunk_buffer>( c, Chunk_buffer( false, true, "5", "abc", 2 ) );

	c.clean();
	feed_secure( c, raw, "2\r\n123\r\n" );
	verify_exception();

	c.clean();
	raw = "";
	c.set_limits(5, 5);
	feed_secure( c, raw, "2" );
	feed_secure( c, raw, "\r\n" );
	feed_secure( c, raw, "an" );
	feed_secure( c, raw, "\r\n" );
	verify_assert_equal<Chunk_buffer>( c, Chunk_buffer( true, true, "2", "an", 0 ) );

	c.clean();
	raw = "";
	feed_secure( c, raw, "3" );
	feed_secure( c, raw, "\r" );
	feed_secure( c, raw, "\n" );
	verify_assert_equal<Chunk_buffer>( c, Chunk_buffer( false, true, "3", "", 3 ) );

	c.clean();
	raw = "";
	feed_secure( c, raw, "2" );
	feed_secure( c, raw, "\r" );
	feed_secure( c, raw, "\n" );
	feed_secure( c, raw, "a" );
	feed_secure( c, raw, "n" );
	feed_secure( c, raw, "\r" );
	feed_secure( c, raw, "\n" );
	verify_assert_equal<Chunk_buffer>( c, Chunk_buffer( true, true, "2", "an", 0 ) );
	verify_assert_bool( c.is_last() == false );
	verify_assert_bool( raw == "" );

	c.clean();
	raw = "";
	feed_secure( c, raw, "2" );
	feed_secure( c, raw, "\r" );
	feed_secure( c, raw, "\n" );
	feed_secure( c, raw, "a" );
	feed_secure( c, raw, "n" );
	feed_secure( c, raw, "\r" );
	verify_assert_equal<Chunk_buffer>( c, Chunk_buffer( false, true, "2", "an", 0 ) );

	c.clean();
	raw = "";
	feed_secure( c, raw, "2" );
	feed_secure( c, raw, "\r" );
	feed_secure( c, raw, "\n" );
	feed_secure( c, raw, "a" );
	feed_secure( c, raw, "n" );
	feed_secure( c, raw, "b" );
	feed_secure( c, raw, "a" );
	verify_exception();

	c.clean();
	raw = "";
	feed_secure( c, raw, "0\r\n\r\n" );
	verify_assert_bool( c.is_last() == true );

	c.clean();
	raw = "";
	feed_secure( c, raw, "0" );
	feed_secure( c, raw, "\r" );
	feed_secure( c, raw, "\n" );
	feed_secure( c, raw, "\r" );
	feed_secure( c, raw, "\n" );
	verify_assert_bool( c.is_last() == true );
	verify_assert_bool( raw == "" );

	c.clean();
	raw = "";
	feed_secure( c, raw, "0" );
	feed_secure( c, raw, "\r" );
	feed_secure( c, raw, "\n" );
	feed_secure( c, raw, "0" );
	feed_secure( c, raw, "\r" );
	feed_secure( c, raw, "\n" );
	verify_exception();

	c.clean();
	raw = "";
	feed_secure( c, raw, "1q\r\nww\r\n" );
	verify_exception();
	
	c.clean();
	raw = "";
	feed_secure( c, raw, "\r\nww\r\n" );
	verify_exception();

	/* max head size */
	c.clean();
	c.set_limits(5, 5);
	feed_secure( c, raw, "123456" );
	verify_exception();

	c.clean();
	c.set_limits(5, 5);
	feed_secure( c, raw, "7\r\n" );
	verify_exception();

	c.clean();
	feed_secure( c, raw, "" );
	verify_assert_equal<Chunk_buffer>( c, Chunk_buffer( false, false, "", "", 0 ) );

	c.clean();
	raw = "";
	feed_secure( c, raw, "" );
	feed_secure( c, raw, "" );
	feed_secure( c, raw, "" );
	feed_secure( c, raw, "" );
	feed_secure( c, raw, "2" );
	feed_secure( c, raw, "\r" );
	feed_secure( c, raw, "\nan\r\n" );
	verify_assert_equal<Chunk_buffer>( c, Chunk_buffer( true, true, "2", "an", 0 ) );
	verify_assert_bool( raw == "" );

	std::cout << result_info( GET_SUCCESS )
	<< " test(s) pass over " << result_info( GET_TEST ) << " test(s)." << std::endl;
	/* 
	test_o<test_struct> t;
	test_struct res;

	res.test( raw );
	t.tag( "valid" );
	t.name( "valid_2" );
	t.verify_assert( res, test_struct( true, 1, "A", 6 ) );
	*/
}