/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   serveur.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: datack <datack@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/26 14:47:13 by datack            #+#    #+#             */
/*   Updated: 2022/05/26 15:23:12 by datack           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "serveur.hpp"

Serveur::Serveur(void)
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

std::list<int> Serveur::getPort()
{
	return this->port;
}
std::vector<Route> Serveur::getRoutes()
{
	return this->routes;
}
std::string Serveur::getName()
{
	return this->server_name;
}

std::vector<std::string> Serveur::getIndex()
{
	return this->index;
}

void Serveur::addPort(int port)
{
	this->port.push_back(port);
}
void Serveur::addRoute(Route route)
{
	this->routes.push_back(route);
}
void Serveur::setName(std::string server_name)
{
	this->server_name.clear();
	this->server_name.append(server_name);
}
void Serveur::addIndex(std::string index)
{
	this->index.push_back(index);
}