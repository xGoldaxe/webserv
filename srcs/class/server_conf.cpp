/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server_conf.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: datack <datack@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/26 14:47:13 by datack            #+#    #+#             */
/*   Updated: 2022/05/27 13:17:32 by datack           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server_conf.hpp"

Server_conf::Server_conf(void)
{
	this->server_name = "webserv (42) v0.1-dev";
	this->port.push_back(3000);
	this->index.push_back("index.html");
	Route route1("/", "./www");
	Route route2("/php", "./cgi");
	routes.push_back(route1);
	routes.push_back(route2);
	routes.back().enable_cgi("/usr/bin/php");
	routes.at(0).add_error_page(404, "defaultPages/404.html");
}

std::list<int> Server_conf::getPort() const
{
	return this->port;
}
std::vector<Route> Server_conf::getRoutes() const
{
	return this->routes;
}
std::string Server_conf::getName() const
{
	return this->server_name;
}

std::vector<std::string> Server_conf::getIndex() const
{
	return this->index;
}

void Server_conf::addPort(int port)
{
	this->port.push_back(port);
}
void Server_conf::addRoute(Route route)
{
	this->routes.push_back(route);
}
void Server_conf::setName(std::string server_name)
{
	this->server_name.clear();
	this->server_name.append(server_name);
}
void Server_conf::addIndex(std::string index)
{
	this->index.push_back(index);
}