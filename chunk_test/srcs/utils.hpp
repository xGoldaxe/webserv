// #ifndef UTILS_HPP
// #	define UTILS_HPP

// #include <iostream>
// #include <string>
// #include <vector>





	
// std::vector<std::string>	read_until( std::string & data, bool (*rule)(std::string), int max = -1 ) {

// 	std::string actual;
// 	std::vector<std::string> res;

// 	int i = 0;
// 	for ( std::string::iterator it = data.begin(); it != data.end(); ++it )
// 	{
// 		actual += *it;
// 		if ( rule(actual) )
// 		{
// 			res.push_back( actual );
// 			actual = "";
// 			++i;
// 			if ( max != -1 && i >= max )
// 				break ;
// 		}
// 	}
// 	return res;
// }


// #endif