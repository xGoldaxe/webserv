#include "../webserv.hpp"

bool is_file(const char* name)
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

inline bool file_exist(const std::string& name) {
	struct stat buffer;   
	return (stat (name.c_str(), &buffer) == 0); 
}


Request::Request( std::string raw_data, webserv_conf &conf ) : conf(conf) {
	
	std::vector<std::string> splitted_str;

	// split the first line
	std::string first_line = raw_data.substr(0, raw_data.find("\n"));
	std::stringstream stream_str(first_line);
	std::string buffer_str;
	while (std::getline(stream_str, buffer_str, ' '))
		splitted_str.push_back(buffer_str);
	
	//file informations
	this->method = splitted_str.at(0);
	this->url = splitted_str.at(1);
	this->version = splitted_str.at(2);
	this->row_data = raw_data;
};
Request::~Request( void ) {};

/* end coplien */
std::string	Request::getMethod(void) {
	return (method);
}
std::string Request::getBody(void) {
	return (body);
}
std::string Request::getUrl(void) {
	return (url);
}
std::string Request::getRelativeUrl(void) {
	return (this->conf.root + url);
}
std::string Request::tryUrl( int *status ) {

	std::string actual_url = this->getRelativeUrl();
	if ( file_exist(actual_url) && is_file( actual_url.c_str() ) )
	{
		*status = 200;
		return (actual_url);
	}
	for ( std::vector<std::string>::iterator it = this->conf.index.begin();
		it != this->conf.index.end() ; ++it )
	{
		if ( this->getUrl().at( this->getUrl().size() - 1 ) == '/' )
			actual_url = this->getRelativeUrl() + *it;
		else
			actual_url = this->getRelativeUrl() + '/' + *it;

		std::cout << " try... "  << actual_url << std::endl;

		if ( file_exist(actual_url)  && is_file( actual_url.c_str() ) )
		{
			*status = 200;
			return (actual_url);
		}
	}
	*status = 404;
	return ( this->getRelativeUrl() );
}