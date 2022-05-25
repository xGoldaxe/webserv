#ifndef WEBSERV_CONF_HPP
#	define WEBSERV_CONF_HPP

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include "route.hpp"
#include <list>

struct Webserv_conf
{
	std::string 				root;
	std::vector<std::string>	index;
	std::string 				http_version;
	std::string					server_name;
	std::vector<Route>			routes;
	std::list<int>				port; //Choisir le port; ecouter plusieurs ports

	Webserv_conf(void);
	Webserv_conf(std::string filename);

	void	print_conf(void);

/* exceptions */
        class FailedToOpenFile : public std::exception	{
            public:
                virtual const	char* what() const throw()	{
                    return ("Webserv_conf::Webserv_conf Couldn't open file");
                }
        };

};


#endif