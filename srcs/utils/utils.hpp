#pragma once

#include <iostream>
#include <string>
#include <map>
#include <vector>

bool	is_full( std::string s, std::string charset );
bool	is_full_hexa( std::string s );
bool	check_line( std::string str );
bool	is_space( char c );
bool	check_and_trunc_line( std::string *line );
std::string test_through_str( std::string &str, bool (*rule)(std::string) );
int hexToInt(std::string hex);
bool is_more_ll( const std::string &str );

std::vector<std::string>	        split_params( const std::string & raw );
std::map<std::string, std::string>  parse_params( std::vector<std::string>::iterator begin,
                                                    std::vector<std::string>::iterator end );

#include <queue>
template < class T >
void	delete_queue( std::queue<T*> q )
{
	while ( !q.empty() )
	{
        delete q.front();
        q.pop();
    }
}