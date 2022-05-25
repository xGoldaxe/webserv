/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv_conf.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: datack <datack@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/25 16:05:17 by datack            #+#    #+#             */
/*   Updated: 2022/05/25 17:24:05 by datack           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv_conf.hpp"
#include "../webserv.hpp"

//default values
Webserv_conf::Webserv_conf(void)
{	
		port.push_front(3000);
		root = ".";
		index.push_back("index.html");
		http_version = "HTTP/1.1";
		server_name = "webserv (42) v0.1-dev";
		Route route1( "/", "./www" );
		Route route2( "/php", "./cgi" );
		routes.push_back( route1 );
		routes.push_back( route2 );
		routes.back().enable_cgi( "/usr/bin/php" );
		routes.at(0).add_error_page( 404, "defaultPages/404.html");
};

Webserv_conf::Webserv_conf(std::string filename)
{
	root = ".";
	
	//default server name "Setup server_name ou pas"
	server_name = "webserv (42) v0.1-dev";

	std::ifstream file;
	std::string buffer;
	file.open(filename.c_str(), std::ifstream::in);
	if (!file.is_open())
	{
		throw Webserv_conf::FailedToOpenFile();
	}
	std::getline(file, buffer);
	while(!file.eof())
	{
		//std::cout << buffer << std::endl;
		
		std::getline(file, buffer);
	}

	file.close();
};

void	Webserv_conf::print_conf(void)
{
	std::cout << "root = " << this->root << std::endl;
	std::cout << "http version = " << this->http_version << std::endl;
	std::cout << "server_name = " << this->server_name << std::endl;

	std::cout << "index = " << std::endl;
	for (std::vector<std::string>::const_iterator i = (this->index).begin(); i != (this->index).end(); ++i)
 	   std::cout << *i << ' ';
	std::cout << std::endl;

	//for (std::vector<Route>::const_iterator i = (this->index).begin(); i != path.end(); ++i)
 	//   std::cout << *i << ' ';
	std::cout << "ports = " << std::endl;
	for (std::list<int>::const_iterator k = (this->port).begin(); k != (this->port).end(); ++k)
 	   std::cout << *k << ' ';
	std::cout << std::endl;
}

/*
	std::string 				root;
	std::vector<std::string>	index;
	std::string 				http_version;
	std::string					server_name;
	std::vector<Route>			routes;
	std::list<int>				port; 
*/
