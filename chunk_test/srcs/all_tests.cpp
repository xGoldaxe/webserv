#include "chunk.hpp"

void	all_tests()
{
	std::string raw;
	/* incomplete */
	raw = "nothing intressting";
	try_chunk_w( raw );
	verify_assert( test_struct( false ) );

	/* all good */
	raw = "1\r\nH\r\n";
	try_chunk_w( raw );
	verify_assert( test_struct( true, 1, "H", 6 ) );

	/* all good */
	raw = "F\r\n123456789abcdef\r\n";
	try_chunk_w( raw );
	verify_assert( test_struct( true, 15, "123456789abcdef", 20 ) );

	/* invalid hexa */
	raw = "oewq\r\nH\r\n";
	try_chunk_w( raw );
	verify_exception();

	/* unclose body */
	raw = "1\r\nH";
	try_chunk_w( raw );
	verify_assert( test_struct( false ) );

	/* invalid size */
	raw = "2\r\n1\r\n";
	try_chunk_w( raw );
	verify_exception();

	/* invalid size */
	raw = "0\r\n\r\n";
	try_chunk_w( raw );
	verify_assert( test_struct( true, 0, "", 5 ) );

	raw = "\r\n\r\n";
	try_chunk_w( raw );
	verify_exception();

	/* 
	test_o<test_struct> t;
	test_struct res;

	res.test( raw );
	t.tag( "valid" );
	t.name( "valid_2" );
	t.verify_assert( res, test_struct( true, 1, "A", 6 ) );
	*/
}