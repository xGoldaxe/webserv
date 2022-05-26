/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv_conf.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: datack <datack@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/25 16:05:17 by datack            #+#    #+#             */
/*   Updated: 2022/05/26 14:11:20 by datack           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv_conf.hpp"
#include "../webserv.hpp"
#include <cstring>

// default values
Webserv_conf::Webserv_conf(void)
{
	port.push_back(3000);
	root = ".";
	index.push_back("index.html");
	http_version = "HTTP/1.1";
	server_name = "webserv (42) v0.1-dev";
	Route route1("/", "./www");
	Route route2("/php", "./cgi");
	routes.push_back(route1);
	routes.push_back(route2);
	routes.back().enable_cgi("/usr/bin/php");
	routes.at(0).add_error_page(404, "defaultPages/404.html");
};

static int return_type_parse(std::string s)
{
	unsigned int i = 0;
	std::string tab[10] = {"server_name", "listen",
						 "error_page", "location",
						 "root", "index",
						 "methods", "enable_cgi",
						 "cgi_extension", "body_max_size"};
	while (i < 10)
	{
		if (s.compare(tab[i]) == 0)
			return i;
		i++;
	}
	return (-1);
}

Webserv_conf::Webserv_conf(std::string filename)
{
	root = ".";

	// default server name "Setup server_name ou pas"
	server_name = "webserv (42) v0.1-dev";

	std::ifstream file;
	std::string buffer;
	std::stringstream s;
	std::vector<std::string> words;
	size_t pos = 0;
	int check = -1;
	unsigned int it = 0;

	file.open(filename.c_str(), std::ifstream::in);
	if (!file.is_open())
	{
		throw Webserv_conf::FailedToOpenFile();
	}
	s << file.rdbuf();
	buffer = s.str();
	file.close();

	// buffer.erase(std::remove(buffer.begin(), buffer.end(), '\t'), buffer.end());
	std::replace(buffer.begin(), buffer.end(), '\t', ' ');
	std::replace(buffer.begin(), buffer.end(), '{', ' ');
	std::replace(buffer.begin(), buffer.end(), '}', ' ');
	std::replace(buffer.begin(), buffer.end(), '\n', ' ');

	while ((pos = buffer.find(" ")) != std::string::npos)
	{
		words.push_back(buffer.substr(0, pos));
		buffer.erase(0, pos + 1);
	}
	words.erase(std::remove(words.begin(), words.end(), ""), words.end());
	while (it != words.size())
	{
		check = return_type_parse(words[it]);
		std::cout << words[it] << "|"
				  << check << std::endl;
		// TODO:switch depending on check
		it++;
	}
};

void Webserv_conf::print_conf(void)
{
	std::cout << "root = " << this->root << std::endl;
	std::cout << "http version = " << this->http_version << std::endl;
	std::cout << "server_name = " << this->server_name << std::endl;

	std::cout << "index = " << std::endl;
	for (std::vector<std::string>::const_iterator i = (this->index).begin(); i != (this->index).end(); ++i)
		std::cout << *i << ' ';
	std::cout << std::endl;

	// for (std::vector<Route>::const_iterator i = (this->index).begin(); i != path.end(); ++i)
	//    std::cout << *i << ' ';
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
