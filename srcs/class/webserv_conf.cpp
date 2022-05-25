/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv_conf.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: datack <datack@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/25 16:05:17 by datack            #+#    #+#             */
/*   Updated: 2022/05/25 16:57:19 by datack           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv_conf.hpp"
#include "../webserv.hpp"

//default values
Webserv_conf::Webserv_conf(void)
{	
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
	std::ifstream file;
	std::string buffer;
	file.open(filename.c_str(), std::ifstream::in);
	if (!file.is_open())
	{
		throw Webserv_conf::FailedToOpenFile();
	}

	//std::stringstream bufstream;
	//bufstream << file.rdbuf();
	//buffer = bufstream.str();
	std::getline(file, buffer);
	
	while(!file.eof())
	{
		std::cout << buffer << std::endl;
		std::getline(file, buffer);
	}

	file.close();


/*
		index.push_back("index.html");
		http_version = "HTTP/1.1";
		server_name = "webserv (42) v0.1-dev";
		Route route1( "/", "./www" );
		Route route2( "/php", "./cgi" );
		routes.push_back( route1 );
		routes.push_back( route2 );
		routes.back().enable_cgi( "/usr/bin/php" );
		routes.at(0).add_error_page( 404, "defaultPages/404.html");

*/
};
