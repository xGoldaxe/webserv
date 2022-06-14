// #ifndef READ_CHUNK_HPP
// #	define READ_CHUNK_HPP

// #include <iostream>
// #include <string>
// #include <vector>
// #include "../../srcs/errors/http_code.hpp"
// #include "utils.hpp"

// // need to fix recheck every times, we only need to check when additonal data came.
// // this is not exactly how it should behave, need to modify its fast!
// bool	try_chunk( std::string chunk_str, size_t &chunk_size, std::string &content, size_t &whole_chunk_size )
// {
// 	whole_chunk_size = chunk_str.size();
// 	std::string head = test_through_str( chunk_str, &check_line );
// 	std::string::iterator find = chunk_str.begin() + head.size();
// 	if ( find == chunk_str.end() )
// 		return false;
// 	if ( check_and_trunc_line( &head ) )
// 	{
// 		if ( is_full_hexa( head ) == false ) // do not accept chunk extension!
// 			throw HTTPCode400();
// 		chunk_size = hexToInt( head );
// 		size_t diff = std::distance( find , chunk_str.end() ); // +1 is allowed because "find" is not already the end		
// 		if ( diff >= chunk_size + 2 )
// 		{
// 			content = std::string( find, find + chunk_size + 2 );
// 			if ( check_and_trunc_line( &content ) )
// 			{
// 				return true;
// 			}
// 			else
// 				throw HTTPCode400();
// 		}
// 	}
// 	return false;
// }

// #endif