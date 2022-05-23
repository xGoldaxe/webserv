#include "webserv.hpp"
#include <fstream>

std::string to_string( int number ) {

	std::string result;

	std::ostringstream convert;
	convert << number;  
	result = convert.str();
	return ( result );
}

bool	is_file(const char* name)
{
	DIR* directory = opendir(name);

	if(directory != NULL)
	{
		closedir(directory);
		return false;
	}

	if(errno == ENOTDIR)
	{
		return true;
	}

	return false;
}

bool	file_exist(const std::string& name) {
	struct stat	buffer;   
	return (stat (name.c_str(), &buffer) == 0); 
}

bool	file_readable(const std::string &name) {

	std::ifstream	my_file( name.c_str() );
	return ( my_file.good() );
}

std::string	finish_by_only_one( std::string str, char c ) {

	if ( *( str.end() ) == c )
		return str;
	str += c;
	return str;
}

std::string read_binary( std::string filename ) {

	std::ifstream input( filename.c_str(), std::ios::binary );
    std::istreambuf_iterator<char> it(input), end;
    std::string ss(it, end);
	return ss;
}

std::string get_extension(std::string file_name)
{
	int position=file_name.find_last_of(".");
	return file_name.substr(position+1);
}