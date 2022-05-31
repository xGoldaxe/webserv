#include "webserv.hpp"
#include <fstream>
#include <unistd.h>
#include <strings.h>

std::string to_string( int number ) {

	std::string result;

	std::ostringstream convert;
	convert << number;  
	result = convert.str();
	return ( result );
}

int	is_file(const char* name)
{
	DIR* directory = opendir(name);

	if(directory != NULL)
	{
		closedir(directory);
		return 0;
	}

	if(errno == ENOTDIR)
	{
		return 1;
	}
	//doesnt exist
	return -1;
}

bool	file_exist(const std::string& name) {
	struct stat	buffer;   
	return (stat (name.c_str(), &buffer) == 0); 
}

bool	file_readable(const std::string &name) {

	std::ifstream	my_file( name.c_str() );
	return ( my_file.good() );
}

bool	usable_file( const std::string &name ) {

	return ( file_exist( name ) && is_file( name.c_str() ) == 1 && file_readable( name ) );
}

std::string	finish_by_only_one( std::string str, char c ) {

	if ( str.size() == 0 )
		return str;
	if ( str.at( str.size() - 1 ) == c )
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

std::string	read_fd( int fd ) {

	(void)fd;
	char buff[256];
	int readed = 255;
	std::string	res;
	while (readed != 0)
	{
		bzero(buff, 256);
		readed = read( fd, buff, 255 );
		buff[readed] = '\0';
		res += buff;
	}
	return res;
}

std::string get_extension(std::string file_name)
{
	int position = file_name.find_last_of(".");
	return file_name.substr(position+1);
}

std::vector<std::string>	split_str( std::string str ) {

	std::vector<std::string> splitted_str;

	std::stringstream stream_str( str );
	std::string buffer_str;
	while ( std::getline(stream_str, buffer_str, ' ') )
		splitted_str.push_back(buffer_str);
	return	splitted_str;
}